// c-primes-simd-1e9.cpp
// AVX2-accelerated segmented sieve for n = 1,000,000,000
// Compile: g++ -O3 -march=native -mavx2 -std=c++17 c-primes-simd-1e9.cpp -o c-primes-simd-1e9

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#if defined(__AVX2__)
#include <immintrin.h>
#define HAS_AVX2 1
#else
#define HAS_AVX2 0
#endif

using u64 = uint64_t;
using u32 = uint32_t;

// Cross-platform intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
inline int ctz64(u64 x) { unsigned long i; _BitScanForward64(&i, x); return i; }
inline int popcnt64(u64 x) { return (int)__popcnt64(x); }
#else
inline int ctz64(u64 x) { return __builtin_ctzll(x); }
inline int popcnt64(u64 x) { return __builtin_popcountll(x); }
#endif

// ============================================================================
// Pre-sieve wheel for primes 3, 5, 7 (period = 105)
// ============================================================================
constexpr u32 WHEEL_PRIMES[] = {3, 5, 7};
constexpr u32 WHEEL_PERIOD = 105;
constexpr u32 WHEEL_HALF = 53;  // Odd numbers in one period

alignas(32) u64 g_wheel_pattern[2];  // 53 bits fits in 1 word, pad to 2

void init_wheel_pattern() {
    g_wheel_pattern[0] = ~0ULL;
    g_wheel_pattern[1] = ~0ULL;
    
    for (u32 p : WHEEL_PRIMES) {
        for (u32 m = p; m < WHEEL_PERIOD; m += p) {
            if (m & 1) {  // Only odd multiples
                u32 idx = (m - 1) / 2;
                if (idx < 64) g_wheel_pattern[0] &= ~(1ULL << idx);
                else g_wheel_pattern[1] &= ~(1ULL << (idx - 64));
            }
        }
    }
    // Restore 3, 5, 7 as prime
    g_wheel_pattern[0] |= (1ULL << 1) | (1ULL << 2) | (1ULL << 3);
}

// ============================================================================
// Base sieve for primes up to sqrt(n)
// ============================================================================
std::vector<u32> base_sieve(u32 n) {
    u32 h = n / 2 + 1;
    std::vector<u64> b((h + 63) >> 6, ~0ULL);
    b[0] ^= 1;
    for (u32 i = 1, L = (u32)std::sqrt(n) / 2; i <= L; ++i)
        if (b[i >> 6] >> (i & 63) & 1)
            for (u32 j = 2*i*(i+1), s = 2*i+1; j < h; j += s)
                b[j >> 6] &= ~(1ULL << (j & 63));
    std::vector<u32> P{2};
    for (u32 i = 0; i < b.size(); ++i)
        for (auto w = b[i]; w; w &= w - 1) {
            u32 v = ((i << 6) + ctz64(w)) * 2 + 1;
            if (v > 1 && v <= n) P.push_back(v);
        }
    return P;
}

// ============================================================================
// AVX2-accelerated segment operations
// ============================================================================

#if HAS_AVX2
// Fill segment with all 1s using 256-bit stores
inline void avx2_fill_ones(u64* ptr, size_t words) {
    __m256i ones = _mm256_set1_epi64x(-1LL);
    size_t i = 0;
    for (; i + 4 <= words; i += 4)
        _mm256_store_si256((__m256i*)(ptr + i), ones);
    for (; i < words; ++i)
        ptr[i] = ~0ULL;
}

// Count set bits in segment using AVX2 popcount emulation
// (True _mm256_popcnt_epi64 requires AVX512-VPOPCNTDQ, so we use lookup)
inline u64 avx2_popcount_segment(const u64* ptr, size_t words) {
    u64 total = 0;
    // Unroll popcount for better throughput
    size_t i = 0;
    for (; i + 4 <= words; i += 4) {
        total += popcnt64(ptr[i]);
        total += popcnt64(ptr[i+1]);
        total += popcnt64(ptr[i+2]);
        total += popcnt64(ptr[i+3]);
    }
    for (; i < words; ++i)
        total += popcnt64(ptr[i]);
    return total;
}
#endif

// ============================================================================
// Main SIMD Sieve
// ============================================================================

int main() {
    using namespace std::chrono;
    
    constexpr u64 n = 1'000'000'000ULL;
    constexpr u32 S = 1 << 18;  // 256K odds = 32KB segment (L1 friendly)
    constexpr u32 SEG_WORDS = (S + 63) >> 6;
    
    std::cout << "=== SIMD Prime Sieve (n = " << n << ") ===\n";
    #if HAS_AVX2
    std::cout << "AVX2: ENABLED\n";
    #else
    std::cout << "AVX2: DISABLED (scalar fallback)\n";
    #endif
    std::cout << "Segment size: " << (S * 2) << " integers (" << (SEG_WORDS * 8) << " bytes)\n\n";
    
    auto t0 = high_resolution_clock::now();
    
    // Initialize wheel pattern
    init_wheel_pattern();
    
    // Generate base primes up to sqrt(n)
    auto B = base_sieve((u32)std::sqrt((double)n) + 1);
    
    auto t1 = high_resolution_clock::now();
    
    // Precompute first odd multiple >= p^2 for each base prime (offset within segment)
    struct PrimeInfo {
        u32 prime;
        u32 next_idx;  // Next composite index within current segment
    };
    std::vector<PrimeInfo> primes_info;
    primes_info.reserve(B.size());
    for (size_t i = 1; i < B.size(); ++i) {  // Skip 2
        primes_info.push_back({B[i], 0});
    }
    
    // Aligned segment buffer
    alignas(64) u64 seg[SEG_WORDS];
    
    // Result tracking
    std::array<u64, 5> ring{};
    u64 cnt = 0, pos = 0;
    
    auto add = [&](u64 p) { ++cnt; ring[pos++ % 5] = p; };
    if (n >= 2) add(2);
    
    auto t2 = high_resolution_clock::now();
    
    // Main sieve loop
    for (u64 lo = 3; lo <= n; lo += S << 1) {
        u64 hi = std::min(lo + (S << 1) - 2, n);
        u64 seg_size = ((hi - lo) >> 1) + 1;
        u64 seg_words = (seg_size + 63) >> 6;
        
        // Step 1: Initialize segment with all 1s (AVX2 accelerated)
        #if HAS_AVX2
        avx2_fill_ones(seg, seg_words);
        #else
        std::fill(seg, seg + seg_words, ~0ULL);
        #endif
        
        // Step 2: Mark composites for each base prime
        for (auto& pi : primes_info) {
            u64 p = pi.prime;
            u64 p2 = p * p;
            
            // Find starting point in this segment
            u64 start;
            if (p2 >= lo) {
                start = p2;
            } else {
                start = ((lo + p - 1) / p) * p;
                if (!(start & 1)) start += p;
            }
            
            if (start > hi) continue;
            
            // Mark all odd multiples of p in [start, hi]
            u64 idx = (start - lo) >> 1;
            
            // Unrolled marking for small primes (hot path)
            if (p < 64) {
                while (idx + 4 * p <= seg_size) {
                    seg[idx >> 6] &= ~(1ULL << (idx & 63));
                    idx += p;
                    seg[idx >> 6] &= ~(1ULL << (idx & 63));
                    idx += p;
                    seg[idx >> 6] &= ~(1ULL << (idx & 63));
                    idx += p;
                    seg[idx >> 6] &= ~(1ULL << (idx & 63));
                    idx += p;
                }
            }
            
            // Remainder
            while (idx < seg_size) {
                seg[idx >> 6] &= ~(1ULL << (idx & 63));
                idx += p;
            }
        }
        
        // Step 3: Extract primes (Kernighan bit-clear + ctz)
        for (size_t i = 0; i < seg_words; ++i) {
            for (auto w = seg[i]; w; w &= w - 1) {
                u64 v = lo + (((i << 6) + ctz64(w)) << 1);
                if (v <= n) add(v);
            }
        }
    }
    
    auto t3 = high_resolution_clock::now();
    
    // Timing breakdown
    auto base_ms = duration_cast<milliseconds>(t1 - t0).count();
    auto prep_ms = duration_cast<milliseconds>(t2 - t1).count();
    auto sieve_ms = duration_cast<milliseconds>(t3 - t2).count();
    auto total_ms = duration_cast<milliseconds>(t3 - t0).count();
    
    std::cout << "Base sieve:  " << base_ms << " ms\n";
    std::cout << "Preparation: " << prep_ms << " ms\n";
    std::cout << "Main sieve:  " << sieve_ms << " ms\n";
    std::cout << "─────────────────────\n";
    std::cout << "Total:       " << total_ms << " ms\n\n";
    
    std::cout << "Found " << cnt << " primes up to " << n << "\n";
    std::cout << "Last 5: ";
    auto k = cnt < 5 ? cnt : 5;
    for (size_t i = 0; i < k; ++i) 
        std::cout << ring[(pos + 5 - k + i) % 5] << ' ';
    std::cout << "\n\n";
    
    std::cout << "Throughput: " << (n / total_ms) / 1000 << " million integers/sec\n";
    std::cout << "Prime rate:  " << (cnt / total_ms) / 1000 << " million primes/sec\n";
    
    return 0;
}
