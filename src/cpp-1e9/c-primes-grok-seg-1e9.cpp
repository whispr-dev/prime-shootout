#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

#define CLEAR_BIT(arr, idx) (arr[(idx) >> 6] &= ~(1ULL << ((idx) & 63)))
#define TEST_BIT(arr, idx) (arr[(idx) >> 6] & (1ULL << ((idx) & 63)))

std::vector<unsigned long long> sieve_odd(unsigned long long lim) {
    if (lim < 2) return {};
    if (lim == 2) return {2ULL};
    auto size = (lim >> 1) + 1;
    std::vector<unsigned long long> bitset((size + 63) / 64, ~0ULL);
    unsigned long long rt = (unsigned long long)sqrtl(lim), mx = rt >> 1;
    for (unsigned long long i = 1; i <= mx; ++i) {
        if (TEST_BIT(bitset, i)) {
            unsigned long long p = (i << 1) + 1;
            for (unsigned long long m = (p * p) >> 1; m < size; m += p)
                CLEAR_BIT(bitset, m);
        }
    }
    std::vector<unsigned long long> primes; primes.reserve(size / 10);
    primes.push_back(2ULL);
    for (unsigned long long i = 1; i < size; ++i) {
        if (TEST_BIT(bitset, i)) {
            unsigned long long p = (i << 1) + 1;
            if (p > lim) break;
            primes.push_back(p);
        }
    }
    return primes;
}

std::vector<unsigned long long> segmented_sieve(unsigned long long n) {
    if (n < 2) return {};
    unsigned long long lim = (unsigned long long)sqrtl(n);
    auto sieving_primes = sieve_odd(lim);
    std::vector<unsigned long long> primes = std::move(sieving_primes);
    constexpr unsigned long long BLOCK_SIZE = 1ULL << 20;
    for (unsigned long long low = lim + 1; low <= n; low += BLOCK_SIZE) {
        unsigned long long high = std::min(low + BLOCK_SIZE - 1ULL, n);
        unsigned long long seg_bits = high - low + 1;
        unsigned long long words = (seg_bits + 63) / 64;
        std::vector<unsigned long long> sieve(words, ~0ULL);
        for (auto p : primes) {  // sieving_primes moved, but same
            if (p * p > high) break;
            unsigned long long first = p * ((low + p - 1ULL) / p);
            if (first < p * p) first = p * p;
            for (unsigned long long j = first; j <= high; j += p)
                CLEAR_BIT(sieve, j - low);
        }
        for (unsigned long long i = 0; i < seg_bits; ++i)
            if (TEST_BIT(sieve, i)) primes.push_back(low + i);
    }
    return primes;
}

int main() {
    unsigned long long n = 1000000000ULL;
    auto start = std::chrono::high_resolution_clock::now();
    auto primes = segmented_sieve(n);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Found " << primes.size() << " primes up to " << n
              << " in " << std::chrono::duration<double>(end - start).count() << "s.\n";
    if (!primes.empty()) {
        std::cout << "Last few: ";
        for (size_t i = primes.size() > 5 ? primes.size() - 5 : 0; i < primes.size(); ++i)
            std::cout << primes[i] << ' ';
        std::cout << '\n';
    }
    return 0;
}
