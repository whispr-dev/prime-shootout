#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using u64 = uint64_t;
using u32 = uint32_t;

// Wheel-30: skip multiples of 2, 3, 5. Only 8/30 numbers can be prime.
constexpr u32 WHEEL[] = {1,7,11,13,17,19,23,29};
constexpr u32 GAPS[] = {6,4,2,4,2,4,6,2};  // gaps between wheel positions

int main() {
    constexpr u64 n = 1'000'000'000ULL;
    u64 sqrt_n = (u64)std::sqrt((double)n) + 1;

    // Simple sieve for base primes
    std::vector<bool> is_small(sqrt_n + 1, true);
    is_small[0] = is_small[1] = false;
    for (u64 p = 2; p * p <= sqrt_n; ++p)
        if (is_small[p])
            for (u64 j = p * p; j <= sqrt_n; j += p)
                is_small[j] = false;

    std::vector<u32> base;
    for (u32 i = 2; i <= sqrt_n; ++i)
        if (is_small[i]) base.push_back(i);

    // Segmented sieve with wheel-30
    constexpr u32 seg_size = 1 << 15;  // 32K segment
    std::vector<bool> seg(seg_size);

    u64 cnt = 0;
    u64 last5[5] = {};
    u32 pos = 0;

    auto add = [&](u64 p) { ++cnt; last5[pos++ % 5] = p; };
    if (n >= 2) add(2);
    if (n >= 3) add(3);
    if (n >= 5) add(5);

    for (u64 lo = 7; lo <= n; lo += seg_size) {
        u64 hi = std::min(lo + seg_size - 1, n);
        std::fill(seg.begin(), seg.end(), true);

        for (u32 p : base) {
            if (p < 7) continue;
            u64 start = ((lo + p - 1) / p) * p;
            if (start < (u64)p * p) start = (u64)p * p;
            for (u64 j = start; j <= hi; j += p)
                seg[j - lo] = false;
        }

        for (u64 i = lo; i <= hi; ++i) {
            u32 rem = i % 30;
            bool on_wheel = (rem==1||rem==7||rem==11||rem==13||rem==17||rem==19||rem==23||rem==29);
            if (on_wheel && seg[i - lo]) add(i);
        }
    }

    std::cout << "Found " << cnt << " primes up to " << n << ".\nLast 5: ";
    for (u32 i = 0; i < 5; ++i) std::cout << last5[(pos - 5 + i + 5) % 5] << ' ';
    std::cout << '\n';
}
