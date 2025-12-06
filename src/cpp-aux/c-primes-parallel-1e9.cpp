#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

using u64 = uint64_t;
using u32 = uint32_t;

int main() {
    constexpr u64 n = 1'000'000'000ULL;
    constexpr u32 seg_size = 131072;
    u32 sqrt_n = (u32)std::sqrt((double)n);

    std::vector<bool> is_small(sqrt_n + 1, true);
    is_small[0] = is_small[1] = false;
    for (u32 p = 2; (u64)p * p <= sqrt_n; ++p)
        if (is_small[p])
            for (u32 i = p * p; i <= sqrt_n; i += p)
                is_small[i] = false;

    std::vector<u32> base;
    for (u32 i = 2; i <= sqrt_n; ++i)
        if (is_small[i]) base.push_back(i);

    u32 num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;

    std::atomic<u64> next_lo{sqrt_n + 1};
    std::vector<u64> thread_counts(num_threads, 0);

    auto worker = [&](u32 tid) {
        std::vector<bool> seg(seg_size);
        while (true) {
            u64 lo = next_lo.fetch_add(seg_size);
            if (lo > n) break;
            u64 hi = std::min(lo + seg_size - 1, n);

            std::fill(seg.begin(), seg.end(), true);
            for (u32 p : base) {
                u64 start = ((lo + p - 1) / p) * p;
                if (start < (u64)p * p) start = (u64)p * p;
                for (u64 j = start; j <= hi; j += p)
                    seg[j - lo] = false;
            }

            for (u64 i = 0; i <= hi - lo; ++i)
                if (seg[i]) ++thread_counts[tid];
        }
    };

    std::vector<std::thread> threads;
    for (u32 i = 0; i < num_threads; ++i)
        threads.emplace_back(worker, i);
    for (auto& t : threads)
        t.join();

    u64 cnt = base.size();
    for (auto c : thread_counts) cnt += c;

    // Single-threaded pass to get last 5 primes
    u64 last5[5] = {};
    u32 pos = 0;
    std::vector<bool> seg(seg_size);
    for (u64 lo = n - seg_size + 1; lo <= n; lo = n + 1) {
        if (lo < sqrt_n + 1) lo = sqrt_n + 1;
        u64 hi = n;
        std::fill(seg.begin(), seg.end(), true);
        for (u32 p : base) {
            u64 start = ((lo + p - 1) / p) * p;
            if (start < (u64)p * p) start = (u64)p * p;
            for (u64 j = start; j <= hi; j += p)
                seg[j - lo] = false;
        }
        for (u64 i = 0; i <= hi - lo; ++i)
            if (seg[i]) last5[pos++ % 5] = lo + i;
        break;
    }

    std::cout << "Found " << cnt << " primes up to " << n << ".\nLast 5: ";
    for (u32 i = 0; i < 5; ++i) std::cout << last5[(pos - 5 + i + 5) % 5] << ' ';
    std::cout << '\n';
}
