#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

using u64 = uint64_t;
using u32 = uint32_t;

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
            u32 v = ((i << 6) + __builtin_ctzll(w)) * 2 + 1;
            if (v > 1 && v <= n) P.push_back(v);
        }
    return P;
}

// ============================================================================
// Segmented Bit-Packed Sieve (Single-threaded, cache-friendly)
// ============================================================================
u64 sieve_segmented(u64 n, u64* last5) {
    constexpr u32 S = 1 << 17;  // 128K odds = 16KB segment (L1 cache)
    auto B = base_sieve((u32)std::sqrt((double)n) + 1);
    std::vector<u64> seg((S + 63) >> 6);
    u64 cnt = 0, pos = 0;

    auto add = [&](u64 p) { ++cnt; last5[pos++ % 5] = p; };
    if (n >= 2) add(2);

    for (u64 lo = 3; lo <= n; lo += S << 1) {
        u64 hi = std::min(lo + (S << 1) - 2, n);
        std::fill(seg.begin(), seg.end(), ~0ULL);
        for (size_t i = 1; i < B.size(); ++i) {
            u64 p = B[i], s = std::max(p * p, ((lo + p - 1) / p) * p);
            if (!(s & 1)) s += p;
            for (u64 j = s; j <= hi; j += p << 1)
                seg[(j - lo) >> 7] &= ~(1ULL << (((j - lo) >> 1) & 63));
        }
        for (size_t i = 0; i < seg.size(); ++i)
            for (auto w = seg[i]; w; w &= w - 1) {
                u64 v = lo + (((i << 6) + __builtin_ctzll(w)) << 1);
                if (v <= n) add(v);
            }
    }
    return cnt;
}

// ============================================================================
// Parallel Segmented Sieve (Multi-threaded counting, faster for large n)
// ============================================================================
u64 sieve_parallel(u64 n, u64* last5) {
    constexpr u32 seg_size = 1 << 18;  // 256K per segment
    u32 sqrt_n = (u32)std::sqrt((double)n);
    auto base = base_sieve(sqrt_n + 1);

    u32 num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;

    std::atomic<u64> next_lo{sqrt_n + 1};
    std::vector<u64> thread_counts(num_threads, 0);

    auto worker = [&](u32 tid) {
        std::vector<u64> seg((seg_size + 127) >> 7);
        while (true) {
            u64 lo = next_lo.fetch_add(seg_size);
            if (lo > n) break;
            u64 hi = std::min(lo + seg_size - 1, n);
            if (!(lo & 1)) ++lo;  // Ensure lo is odd

            std::fill(seg.begin(), seg.end(), ~0ULL);
            for (size_t i = 1; i < base.size(); ++i) {
                u64 p = base[i], s = std::max(p * p, ((lo + p - 1) / p) * p);
                if (!(s & 1)) s += p;
                for (u64 j = s; j <= hi; j += p << 1)
                    seg[(j - lo) >> 7] &= ~(1ULL << (((j - lo) >> 1) & 63));
            }
            for (size_t i = 0; i < seg.size(); ++i)
                for (auto w = seg[i]; w; w &= w - 1) {
                    u64 v = lo + (((i << 6) + __builtin_ctzll(w)) << 1);
                    if (v <= n) ++thread_counts[tid];
                }
        }
    };

    std::vector<std::thread> threads;
    for (u32 i = 0; i < num_threads; ++i)
        threads.emplace_back(worker, i);
    for (auto& t : threads)
        t.join();

    u64 cnt = base.size();
    for (auto c : thread_counts) cnt += c;

    // Get last 5 primes (single-threaded tail scan)
    constexpr u32 tail = 1 << 16;
    std::vector<u64> seg((tail + 127) >> 7);
    u64 lo = (n > tail) ? n - tail + 1 : 3;
    if (!(lo & 1)) ++lo;
    u64 hi = n;
    std::fill(seg.begin(), seg.end(), ~0ULL);
    for (size_t i = 1; i < base.size(); ++i) {
        u64 p = base[i], s = std::max(p * p, ((lo + p - 1) / p) * p);
        if (!(s & 1)) s += p;
        for (u64 j = s; j <= hi; j += p << 1)
            seg[(j - lo) >> 7] &= ~(1ULL << (((j - lo) >> 1) & 63));
    }
    u32 pos = 0;
    for (size_t i = 0; i < seg.size(); ++i)
        for (auto w = seg[i]; w; w &= w - 1) {
            u64 v = lo + (((i << 6) + __builtin_ctzll(w)) << 1);
            if (v <= n) last5[pos++ % 5] = v;
        }
    return cnt;
}

// ============================================================================
// Main
// ============================================================================
int main() {
    using namespace std::chrono;
    constexpr u64 n = 1'000'000'000ULL;
    u32 cores = std::thread::hardware_concurrency();

    std::cout << "The Beast Reborn - n = " << n << ", cores = " << cores << "\n";
    std::cout << std::string(50, '=') << "\n";

    u64 last5[5] = {};
    u64 cnt;

    // Choose strategy based on core count
    auto start = high_resolution_clock::now();
    if (cores >= 4) {
        std::cout << "Strategy: Parallel Segmented\n";
        cnt = sieve_parallel(n, last5);
    } else {
        std::cout << "Strategy: Segmented Bit-Packed\n";
        cnt = sieve_segmented(n, last5);
    }
    auto end = high_resolution_clock::now();

    auto ms = duration_cast<milliseconds>(end - start).count();
    std::cout << "Found " << cnt << " primes in " << ms << " ms\n";
    std::cout << "Rate: " << (n / ms) / 1000 << " million/sec\n";
    std::cout << "Last 5: ";
    for (int i = 0; i < 5; ++i) std::cout << last5[i] << ' ';
    std::cout << '\n';
}
