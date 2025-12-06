# Contributing to Prime-Shootout

Thanks for your interest in improving the benchmarks!

---

## Ways to Contribute

### 1. Add New Implementations

We welcome implementations in any language. Requirements:

- Must implement Sieve of Eratosthenes (or variant)
- Must output prime count and last 5 primes
- Must compile/run on Windows 10/11 (primary platform)
- Should include compilation instructions

**Naming convention:**
```
{language}-primes-{variant}-{author}.{ext}

Examples:
c-primes-avx512-yourname.cpp
rs-primes-rayon-yourname.rs
go-primes-concurrent-yourname.go
```

### 2. Optimize Existing Code

Found a faster approach? Submit a PR with:

1. The optimized implementation
2. Benchmark comparison (before/after)
3. Explanation of the optimization

**Rules:**
- Must produce identical output (same prime count)
- Must not use external libraries for core sieve logic
- Intrinsics (`__builtin_*`, SIMD) are encouraged

### 3. Add New LLM Contestants

Want to test a new LLM? Use the standard prompt:

```
[PERSONA] ROLE: You are the undisputed deity of C++ optimization,
specifically specializing in prime number generation...

[PROMPT] Please improve on the attached .cpp files:
- Fastest (priority)
- Shortest code
- Most elegant

[Attach: c-primes.cpp, c-primes-fast.cpp]
```

Document:
- LLM name and version
- Date tested
- Number of iterations to get working code
- Any notable behaviors

### 4. Improve Benchmarking

The benchmarker (`cs_exe_bm_*.cpp`) can be enhanced:

- Statistical analysis improvements
- Cross-platform support (Linux, macOS)
- Automated result collection
- Visualization generation

---

## Code Style

### C++

```cpp
// Use modern C++ (17/20)
// Prefer fixed-width types
using u64 = uint64_t;
using u32 = uint32_t;

// Descriptive names for public functions
std::vector<u64> sieve_segmented(u64 n);

// Terse names acceptable in hot loops
for (u64 j = s; j <= hi; j += p)

// Document non-obvious bit manipulation
// Kernighan's trick: clears lowest set bit
w &= w - 1;
```

### Rust

```rust
// Follow rustfmt defaults
// Use explicit types for clarity in benchmarks
fn sieve(n: u64) -> Vec<u64>

// Prefer iterators where performance is equivalent
(2..=n).filter(|&i| is_prime[i]).collect()
```

### Python

```python
# NumPy vectorization preferred for large n
# Type hints encouraged
def sieve(n: int) -> list[int]:

# Numba JIT for hot paths
@numba.jit(nopython=True)
def sieve_numba(n):
```

---

## Development Workflow

1. **Fork** the repository

2. **Create a branch:**
   ```bash
   git checkout -b feature/avx512-sieve
   ```

3. **Implement and test:**
   ```bash
   g++ -O3 -march=native your-impl.cpp -o your-impl.exe
   ./your-impl.exe  # Verify output
   ```

4. **Run benchmarks:**
   ```bash
   # Add your exe to the benchmark directory
   cp your-impl.exe build/dist/round-X/
   ./cs_exe_bm_mkviii.exe
   ```

5. **Commit with clear messages:**
   ```
   feat: add AVX-512 vectorized sieve
   
   - Uses 512-bit registers for bulk bit clearing
   - 2.3x speedup on Xeon Platinum
   - Requires -mavx512f flag
   ```

6. **Open a Pull Request** against `main`

---

## Benchmark Requirements

For a result to be included in official tables:

| Requirement | Details |
|-------------|---------|
| Runs | Minimum 10 runs, 10 repeats each |
| Platform | Windows 10/11, or clearly labeled |
| Compiler | GCC 10+ or Clang 12+ |
| Flags | `-O3 -march=native` minimum |
| Verification | Correct prime count |

Include raw benchmark output in your PR.

---

## Testing

Before submitting:

```bash
# Verify correctness
./your-impl.exe | head -1
# Expected: Found 41538 primes up to 500000

# Check for undefined behavior (optional but appreciated)
g++ -fsanitize=undefined your-impl.cpp -o test
./test

# Memory check (optional)
valgrind ./your-impl
```

---

## Questions?

- Open an issue for discussion
- Tag with `question` label
- Check existing issues first

---

**Thanks for helping make Prime-Shootout faster!**
