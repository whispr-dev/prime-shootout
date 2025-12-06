// c-primes-simd-parallel-1e9.cpp
// Ultimate: AVX2 + Multi-threaded segmented sieve
// Compile: g++ -O3 -march=native -mavx2 -pthread -std=c++17 c-primes-simd-parallel-1e9.cpp -o c-primes-simd-parallel-1e9

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

#if defined(__AVX2__)
#include <immintrin.h>
#define HAS_AVX2 1
#else
#define HAS_AVX2 0
#endif

using u64 = uint64_t;
using u32 = uint32_t;

#if defined(_MSC_VER)
#include <intrin.h>
inline int ctz64(u64 x) { unsigned long i; _BitScanForward64(&i, x); return i; }
#else
inline int ctz64(u64 x) { return __builtin_ctzll(x); }
#endif

// Base sieve
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

#if HAS_AVX2
inline void avx2_fill_ones(u64* ptr, size_t words) {
    if (words == 0) return;
    __m256i ones = _mm256_set1_epi64x(-1LL);
    size_t i = 0;
    for (; i + 4 <= words; i += 4)
        _mm256_storeu_si256((__m256i*)(ptr + i), ones);
    for (; i < words; ++i)
        ptr[i] = ~0ULL;
}
#endif

int main() {
    using namespace std::chrono;
    
    constexpr u64 n = 1'000'000'000ULL;
    constexpr u32 S = 1 << 18;  // 256K odds per segment
    constexpr u32 SEG_WORDS = (S + 63) >> 6;
    
    u32 num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    
    std::cout << "=== Ultimate SIMD+Parallel Sieve (n = " << n << ") ===\n";
    #if HAS_AVX2
    std::cout << "AVX2: ENABLED\n";
    #else
    std::cout << "AVX2: DISABLED\n";
    #endif
    std::cout << "Threads: " << num_threads << "\n";
    std::cout << "Segment: " << (SEG_WORDS * 8 / 1024) << " KB\n\n";
    
    auto t0 = high_resolution_clock::now();
    
    // Base primes
    auto B = base_sieve((u32)std::sqrt((double)n) + 1);
    
    auto t1 = high_resolution_clock::now();
    
    // Parallel sieving
    std::atomic<u64> next_lo{3};
    std::vector<u64> thread_counts(num_threads, 0);
    
    auto worker = [&](u32 tid) {
        alignas(64) u64 seg[SEG_WORDS];
        u64 local_cnt = 0;
        
        while (true) {
            u64 lo = next_lo.fetch_add(S << 1);
            if (lo > n) break;
            
            u64 hi = std::min(lo + (S << 1) - 2, n);
            u64 seg_size = ((hi - lo) >> 1) + 1;
            u64 seg_words = (seg_size + 63) >> 6;
            
            // Fill with 1s (AVX2)
            #if HAS_AVX2
            avx2_fill_ones(seg, seg_words);
            #else
            std::fill(seg, seg + seg_words, ~0ULL);
            #endif
            
            // Sieve
            for (size_t i = 1; i < B.size(); ++i) {
                u64 p = B[i];
                u64 start;
                if (p * p >= lo) {
                    start = p * p;
                } else {
                    start = ((lo + p - 1) / p) * p;
                    if (!(start & 1)) start += p;
                }
                if (start > hi) continue;
                
                u64 idx = (start - lo) >> 1;
                
                // Unrolled for small primes
                if (p < 64) {
                    while (idx + 4 * p <= seg_size) {
                        seg[idx >> 6] &= ~(1ULL << (idx & 63)); idx += p;
                        seg[idx >> 6] &= ~(1ULL << (idx & 63)); idx += p;
                        seg[idx >> 6] &= ~(1ULL << (idx & 63)); idx += p;
                        seg[idx >> 6] &= ~(1ULL << (idx & 63)); idx += p;
                    }
                }
                while (idx < seg_size) {
                    seg[idx >> 6] &= ~(1ULL << (idx & 63));
                    idx += p;
                }
            }
            
            // Count primes
            for (size_t i = 0; i < seg_words; ++i)
                for (auto w = seg[i]; w; w &= w - 1)
                    if (lo + (((i << 6) + ctz64(w)) << 1) <= n)
                        ++local_cnt;
        }
        
        thread_counts[tid] = local_cnt;
    };
    
    std::vector<std::thread> threads;
    for (u32 i = 0; i < num_threads; ++i)
        threads.emplace_back(worker, i);
    for (auto& t : threads)
        t.join();
    
    auto t2 = high_resolution_clock::now();
    
    // Sum counts
    u64 cnt = 1;  // Include 2
    for (auto c : thread_counts) cnt += c;
    
    // Get last 5 primes (single-threaded tail scan)
    u64 last5[5] = {};
    u32 pos = 0;
    alignas(64) u64 seg[SEG_WORDS];
    u64 lo = (n > (S << 1)) ? n - (S << 1) + 1 : 3;
    if (!(lo & 1)) ++lo;
    u64 hi = n;
    u64 seg_size = ((hi - lo) >> 1) + 1;
    u64 seg_words = (seg_size + 63) >> 6;
    
    #if HAS_AVX2
    avx2_fill_ones(seg, seg_words);
    #else
    std::fill(seg, seg + seg_words, ~0ULL);
    #endif
    
    for (size_t i = 1; i < B.size(); ++i) {
        u64 p = B[i];
        u64 start;
        if (p * p >= lo) start = p * p;
        else {
            start = ((lo + p - 1) / p) * p;
            if (!(start & 1)) start += p;
        }
        if (start > hi) continue;
        u64 idx = (start - lo) >> 1;
        while (idx < seg_size) {
            seg[idx >> 6] &= ~(1ULL << (idx & 63));
            idx += p;
        }
    }
    
    for (size_t i = 0; i < seg_words; ++i)
        for (auto w = seg[i]; w; w &= w - 1) {
            u64 v = lo + (((i << 6) + ctz64(w)) << 1);
            if (v <= n) last5[pos++ % 5] = v;
        }
    
    auto t3 = high_resolution_clock::now();
    
    auto base_ms = duration_cast<milliseconds>(t1 - t0).count();
    auto sieve_ms = duration_cast<milliseconds>(t2 - t1).count();
    auto tail_ms = duration_cast<milliseconds>(t3 - t2).count();
    auto total_ms = duration_cast<milliseconds>(t3 - t0).count();
    
    std::cout << "Base sieve:     " << base_ms << " ms\n";
    std::cout << "Parallel sieve: " << sieve_ms << " ms\n";
    std::cout << "Tail scan:      " << tail_ms << " ms\n";
    std::cout << "────────────────────────\n";
    std::cout << "Total:          " << total_ms << " ms\n\n";
    
    std::cout << "Found " << cnt << " primes up to " << n << "\n";
    std::cout << "Last 5: ";
    for (int i = 0; i < 5; ++i) std::cout << last5[(pos - 5 + i + 5) % 5] << ' ';
    std::cout << "\n\n";
    
    std::cout << "Throughput: " << (n / (total_ms ? total_ms : 1)) / 1000 << " million/sec\n";
}
