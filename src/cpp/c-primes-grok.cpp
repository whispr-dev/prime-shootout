#include <iostream>
#include <vector>
#include <cmath>

#define CLEAR_BIT(arr, idx) (arr[(idx) >> 6] &= ~(1ULL << ((idx) & 63)))
#define TEST_BIT(arr, idx) (arr[(idx) >> 6] & (1ULL << ((idx) & 63)))

std::vector<unsigned long long> sieve_odd(unsigned long long n) {
    if (n < 2) return {};
    if (n == 2) return {2ULL};
    
    auto size = (n >> 1) + 1;
    std::vector<unsigned long long> bitset((size + 63) / 64, ~0ULL);
    
    unsigned long long sqrt_n = (unsigned long long)sqrtl(n), max_i = sqrt_n >> 1;
    for (unsigned long long i = 1; i <= max_i; ++i) {
        if (TEST_BIT(bitset, i)) {
            unsigned long long p = (i << 1) + 1;
            for (unsigned long long m = (p * p) >> 1; m < size; m += p)
                CLEAR_BIT(bitset, m);
        }
    }
    
    std::vector<unsigned long long> primes;
    primes.reserve(size / 10);
    primes.push_back(2ULL);
    for (unsigned long long i = 1; i < size; ++i) {
        if (TEST_BIT(bitset, i)) {
            unsigned long long p = (i << 1) + 1;
            if (p > n) break;
            primes.push_back(p);
        }
    }
    return primes;
}

int main() {
    unsigned long long n = 500000ULL;
    auto primes = sieve_odd(n);
    std::cout << "Found " << primes.size() << " primes up to " << n << ".\n";
    if (!primes.empty()) {
        std::cout << "Last few: ";
        for (size_t i = primes.size() > 5 ? primes.size() - 5 : 0; i < primes.size(); ++i)
            std::cout << primes[i] << " ";
        std::cout << "\n";
    }
}
