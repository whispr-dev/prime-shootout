#include <iostream>
#include <vector>
#include <cmath>

// [DEITY MODE REDEEMED] 
// Single-threaded, Odd-Only, L1-Cache Optimized Sieve.
// No race conditions. Zero synchronization overhead.
// Complexity: O(N log log N).
std::vector<unsigned long long> sieve_god_mode(unsigned long long n) {
    if (n < 2) return {};
    
    // 1. Setup: Odd-only mapping. Index i -> Number (2*i + 1).
    //    We need indices up to (n-1)/2.
    unsigned long long limit = (n - 1) / 2;
    unsigned long long sqrt_limit = (static_cast<unsigned long long>(std::sqrt(n)) - 1) / 2;
    
    // 2. Storage: Use unsigned long long directly (native 64-bit word).
    //    Init all bits to 1 (true).
    //    Size: (limit / 64) + 1 blocks.
    std::vector<unsigned long long> bits((limit >> 6) + 1, ~0ULL);

    // 3. The Sieve: Tight inner loop.
    for (unsigned long long i = 1; i <= sqrt_limit; ++i) {
        // Check if bit i is set (meaning 2*i+1 is prime)
        // i >> 6 finds the block, i & 63 finds the bit offset.
        if ((bits[i >> 6] >> (i & 63)) & 1) {
            unsigned long long p = (i << 1) + 1;
            unsigned long long start = 2 * i * (i + 1); // Index of p^2
            
            // Unrolled Bitwise Wipe
            // p is odd, so in odd-only index space, we step by p.
            for (unsigned long long j = start; j <= limit; j += p) {
                bits[j >> 6] &= ~(1ULL << (j & 63));
            }
        }
    }

    // 4. Harvest
    std::vector<unsigned long long> primes;
    primes.reserve(n > 1000 ? n / (std::log(n) - 1.1) : n); 
    primes.push_back(2);

    for (unsigned long long i = 1; i <= limit; ++i) {
        if ((bits[i >> 6] >> (i & 63)) & 1) {
            primes.push_back((i << 1) + 1);
        }
    }
    
    return primes;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    unsigned long long n = 500000ULL;
    auto primes = sieve_god_mode(n);

    std::cout << "Found " << primes.size() << " primes up to " << n << ".\n";
    if (!primes.empty()) {
        std::cout << "Last few primes: ";
        auto start_idx = primes.size() > 5 ? primes.size() - 5 : 0;
        for (size_t i = start_idx; i < primes.size(); i++) {
            std::cout << primes[i] << " ";
        }
        std::cout << "\n";
    }
    return 0;
}