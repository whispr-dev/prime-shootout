#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <omp.h>     // OpenMP for parallelism
#include <cstdint>   // Fixed: Required for uint64_t
#include <bit>       // C++20 popcount

// Constants for cache optimization (32KB L1 Data Cache size is common)
constexpr std::size_t L1_CACHE_BYTES = 32768; 
constexpr std::size_t SEGMENT_SIZE = L1_CACHE_BYTES * 8; // in bits (131072 odd numbers)

// Helper: Simple sieve for seed primes up to sqrt(N)
std::vector<unsigned long long> generate_seeds(unsigned long long limit) {
    // Only store odd numbers. Size is roughly (limit/2).
    unsigned long long size = (limit + 1) / 2;
    // Use uint64_t for the bit vector
    std::vector<uint64_t> bits((size + 63) / 64, ~0ULL);
    std::vector<unsigned long long> primes;
    primes.push_back(2);
    
    unsigned long long sqrt_lim = (static_cast<unsigned long long>(std::sqrt(limit)) - 1) / 2;
    
    for (unsigned long long i = 1; i <= sqrt_lim; ++i) {
        if ((bits[i >> 6] >> (i & 63)) & 1) {
            unsigned long long p = (i << 1) + 1;
            // The starting index is p^2 in odd-index space
            for (unsigned long long j = 2 * i * (i + 1); j < size; j += p)
                bits[j >> 6] &= ~(1ULL << (j & 63));
        }
    }
    // Harvest seeds
    for (unsigned long long i = 1; i < size; ++i) {
        if ((bits[i >> 6] >> (i & 63)) & 1) primes.push_back((i << 1) + 1);
    }
    return primes;
}

// The Main Event: Parallel Segmented Sieve
unsigned long long sieve_segmented(unsigned long long n) {
    if (n < 2) return 0;
    
    // 1. Get Seeds
    unsigned long long sqrt_n = static_cast<unsigned long long>(std::sqrt(n));
    auto seeds = generate_seeds(sqrt_n);
    unsigned long long count = seeds.size(); 

    // 2. Partition the rest of the range [sqrt_n + 1, n]
    unsigned long long low_start = sqrt_n + 1;
    if (low_start % 2 == 0) low_start++; // Must start on an odd number

    // Odd numbers in the full range [low_start, n]
    unsigned long long total_odd_count = (n - low_start) / 2 + 1;
    unsigned long long num_segments = (total_odd_count + SEGMENT_SIZE - 1) / SEGMENT_SIZE;

    // Use OpenMP for parallel execution with reduction for the count.
    #pragma omp parallel reduction(+:count)
    {
        // Thread-local sieve segment. Fits in L1 Cache.
        std::vector<uint64_t> segment((SEGMENT_SIZE + 63) / 64);
        
        #pragma omp for schedule(dynamic)
        for (unsigned long long k = 0; k < num_segments; ++k) {
            
            // --- Segment Setup ---
            unsigned long long seg_low_num = low_start + k * SEGMENT_SIZE * 2;
            
            // Calculate actual number of bits to process in this segment.
            unsigned long long current_segment_bits = SEGMENT_SIZE;
            if (k == num_segments - 1) {
                current_segment_bits = total_odd_count - k * SEGMENT_SIZE;
            }

            // Reset segment bits to 1 (all prime)
            std::fill(segment.begin(), segment.end(), ~0ULL);

            // --- Sieve Pass: Clear multiples using seeds ---
            for (unsigned long long p : seeds) { 
                // Optimization: stop if p^2 is beyond the segment's end
                if (p * p > n || p * p > seg_low_num + 2 * current_segment_bits) break;

                // Find first odd multiple of p >= seg_low_num
                unsigned long long start_num = (p * p >= seg_low_num) ? p * p : 
                    (seg_low_num + p - 1) / p * p;
                
                if (start_num % 2 == 0) start_num += p; // Ensure start_num is odd

                // Map 'start_num' to bit index in this segment
                unsigned long long first_idx = (start_num - seg_low_num) / 2;

                // Unrolled Loop Wipe
                for (unsigned long long j = first_idx; j < current_segment_bits; j += p) {
                    segment[j >> 6] &= ~(1ULL << (j & 63));
                }
            }

            // --- Count Primes in Segment using hardware POPCNT ---
            unsigned long long full_blocks = current_segment_bits / 64;
            for (unsigned long long b = 0; b < full_blocks; ++b) {
                count += std::popcount(segment[b]);
            }
            
            // Handle remaining bits in the last partial block
            unsigned long long remaining = current_segment_bits % 64;
            if (remaining > 0) {
                uint64_t mask = (1ULL << remaining) - 1;
                count += std::popcount(segment[full_blocks] & mask);
            }
        }
    }

    return count;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Example: 1 Billion (1e9)
    unsigned long long n = 1000000000ULL; 
    
    std::cout << "Sieving up to " << n << " in parallel segments...\n";
    
    // Check for OpenMP availability and set max threads
    int max_threads = omp_get_max_threads();
    omp_set_num_threads(max_threads);
    std::cout << "Using " << max_threads << " threads.\n";
    
    double start = omp_get_wtime();
    
    unsigned long long count = sieve_segmented(n);
    
    double end = omp_get_wtime();
    
    // Primes up to 10^9 should be 50,847,534
    std::cout << "Primes found: " << count << "\n";
    std::cout << "Time taken: " << (end - start) << "s\n";
    
    return 0;
}