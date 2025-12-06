# Prime-Shootout Changelog

All notable changes to this project are documented here.

---

## v3.1.1 - Round Aux Complete (2025-01-XX)

**Specialized implementations benchmarked**

### Added
- `c-primes-bitpacked-1e9.cpp` — Segmented sieve with Kernighan extraction
- `c-primes-parallel-1e9.cpp` — Multi-threaded atomic work-stealing
- `c-primes-wheel-1e9.cpp` — Wheel-30 factorization sieve
- `c-primes-segment-sieve-1e9.cpp` — Basic segmented with vector<bool>
- `the-beast-reborn-1e9.cpp` — Auto-selecting strategy (parallel if >= 4 cores)
- `cs_exe_bm_mkviii.cpp` — Auxiliary round benchmarker

### Results
- Parallel implementation fastest on multi-core (~800ms on 8 cores)
- Bit-packed single-threaded: ~2,000ms
- Wheel-30: ~2,300ms (memory savings offset by lookup overhead)

---

## v3.1.0 - Round 2 Complete (2025-01-XX)

**Billion-scale benchmarks**

### Added
- All LLM implementations scaled to n = 1,000,000,000
- Segmented sieve variants for each LLM
- `cs_exe_bm_mkvii.cpp` — Round 2 benchmarker with timeout handling
- Rust 1e9 implementations

### Technical Changes
- Migrated from `int` to `uint64_t` to prevent overflow at p*p
- Segmented approach mandatory (125MB -> 30KB memory)
- Added proper `u64` casting for arithmetic operations

### Results
- Claude segmented: ~2,100ms average
- GPetey segmented: ~2,200ms average
- All implementations verified: 50,847,534 primes

### Bug Fixes
- Fixed `std::min` type deduction failures on GCC 13
- Fixed circular buffer indexing for "last 5 primes" display
- Corrected wheel-30 mapping functions

---

## v3.0.0 - Round 1 Complete (2025-01-XX)

**Initial LLM shootout**

### Added
- Claude Opus 4.5 implementation
- ChatGPT 5.1 (GPetey) implementation
- Gemini 3 Pro implementation
- Grok 4.1 implementation
- Baseline C++ implementations (basic + fast)
- Rust implementations (basic + fast)
- Python implementations (basic, numpy, numba, smart)
- `cs_exe_bm_mkvi.cpp` — Round 1 benchmarker

### Prompt Design
All LLMs received identical prompt:
```
[PERSONA] You are the undisputed deity of C++ optimization...
[CHALLENGE] Improve on the attached .cpp files
[CRITERIA] Fastest > Shortest > Most Elegant
```

### Results (n = 500,000)
| LLM | Avg (ms) | Lines |
|-----|----------|-------|
| Claude | 21.36 | 18 |
| GPetey | 21.65 | 45 |
| Gemini | 23.10 | 52 |
| Grok | 22.80 | 48 |

### Scoring
- **Speed (10 pts):** Claude (fastest average)
- **Compactness (10 pts):** Claude (18 lines)
- **Elegance (10 pts):** GPetey (most readable)
- **Final:** Claude 20, GPetey 10

---

## v2.0.0 - Baseline Implementations (2025-01-XX)

**Reference implementations established**

### Added
- `c-primes.cpp` — Basic C++ sieve (vector<bool>)
- `c-primes-fast.cpp` — Optimized C++ (odd-only, bit-packed, OpenMP)
- `r-primes.rs` — Basic Rust sieve
- `r-primes-fast.rs` — Optimized Rust (odd-only, bit manipulation)
- Python hierarchy (basic -> numpy -> numba -> smart)

### Baseline Results (n = 500,000)
- c-primes: ~29ms
- c-primes-fast: ~25ms
- r-primes: ~28ms
- r-primes-fast: ~27ms

---

## v1.0.0 - Project Initialization (2025-01-XX)

**Repository created**

### Added
- Project structure
- Benchmarking framework design
- Documentation templates
- MIT + CC0 hybrid license

---

## Roadmap

### Planned
- [ ] AVX-512 vectorized sieve
- [ ] GPU compute (CUDA/OpenCL) implementation
- [ ] WASM compilation for browser benchmarks
- [ ] Automated CI benchmark regression testing
- [ ] Interactive visualization of results

### Under Consideration
- [ ] Sieve of Atkin comparison
- [ ] Primality testing (Miller-Rabin) benchmarks
- [ ] Memory bandwidth analysis
- [ ] Cross-platform results (Linux, macOS, ARM)

---

## Statistics

| Metric | Value |
|--------|-------|
| Total Implementations | 25+ |
| Languages | 3 (C++, Rust, Python) |
| LLMs Tested | 4 |
| Benchmark Runs | 1000+ |
| Lines of Code | ~3,500 |

---

**Built by whisprer**

*"Measuring intelligence, one prime at a time."*
