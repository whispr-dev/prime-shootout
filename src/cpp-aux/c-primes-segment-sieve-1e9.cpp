#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using u64 = uint64_t;
using u32 = uint32_t;

int main() {
    constexpr u64 n = 1'000'000'000ULL;
    constexpr u32 seg_size = 32768;
    u32 sqrt_n = (u32)std::sqrt((double)n);

    std::vector<bool> is_small(sqrt_n + 1, true);
    is_small[0] = is_small[1] = false;
    for (u32 p = 2; p * p <= sqrt_n; ++p)
        if (is_small[p])
            for (u32 i = p * p; i <= sqrt_n; i += p)
                is_small[i] = false;

    std::vector<u32> base;
    for (u32 i = 2; i <= sqrt_n; ++i)
        if (is_small[i]) base.push_back(i);

    u64 cnt = base.size();
    u64 last5[5] = {};
    u32 pos = 0;
    for (auto p : base) last5[pos++ % 5] = p;

    std::vector<bool> seg(seg_size);
    for (u64 lo = sqrt_n + 1; lo <= n; lo += seg_size) {
        u64 hi = std::min(lo + seg_size - 1, n);
        std::fill(seg.begin(), seg.end(), true);

        for (u32 p : base) {
            u64 start = ((lo + p - 1) / p) * p;
            if (start == p) start = (u64)p * p;
            for (u64 j = start; j <= hi; j += p)
                seg[j - lo] = false;
        }

        for (u64 i = lo; i <= hi; ++i)
            if (seg[i - lo]) {
                ++cnt;
                last5[pos++ % 5] = i;
            }
    }

    std::cout << "Found " << cnt << " primes up to " << n << ".\nLast 5: ";
    u32 k = cnt < 5 ? (u32)cnt : 5;
    for (u32 i = 0; i < k; ++i) std::cout << last5[(pos + 5 - k + i) % 5] << ' ';
    std::cout << '\n';
}
