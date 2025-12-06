# Security Policy

## Scope

This project is a **benchmarking suite** for prime number algorithms. It does not:
- Handle user authentication
- Process sensitive data
- Provide network services
- Execute untrusted code

The security considerations are minimal but documented here for completeness.

---

## Supported Versions

| Version | Supported |
|---------|-----------|
| 3.x.x | Active |
| 2.x.x | Maintenance only |
| < 2.0 | Unsupported |

---

## Potential Concerns

### Integer Overflow

Prime sieve implementations must handle large values carefully:

```cpp
// BAD: Overflows at n > ~46,340
int p_squared = p * p;

// GOOD: Use appropriate types
uint64_t p_squared = (uint64_t)p * p;
```

All Round 2 (1e9) implementations have been audited for overflow safety.

### Buffer Overflow

Bit manipulation macros must validate indices:

```cpp
// Ensure idx < array_size * 64
#define SET_BIT(arr, idx) (arr[(idx) >> 6] |= (1ULL << ((idx) & 63)))
```

All implementations bounds-check against the declared sieve size.

### Denial of Service

The benchmarker limits execution time:
- Default timeout: 60 seconds per implementation
- Memory limit: Implicit via segmented sieve design

Users running with n > 10^12 should expect extended runtimes.

---

## Reporting Issues

If you discover a bug that could cause:
- Incorrect prime counts
- Crashes on valid input
- Undefined behavior

Please open a GitHub issue with:
1. Input parameters (n value)
2. Implementation file
3. Compiler version and flags
4. Expected vs actual output

For security-sensitive issues (unlikely in this project), contact:
- Email: security@whispr.dev
- Response time: 72 hours

---

## Verification

All implementations are verified against known values:

| n | π(n) | Last Prime |
|---|------|------------|
| 500,000 | 41,538 | 499,979 |
| 1,000,000,000 | 50,847,534 | 999,999,937 |

Run verification:

```bash
./c-primes-claude.exe | grep "Found"
# Expected: Found 41538 primes...
```

---

## Build Reproducibility

For reproducible builds, pin compiler versions:

```bash
# GCC
g++ --version  # Tested: 13.2.0

# Flags used
g++ -O3 -march=native -std=c++17 -flto
```

Different optimization levels may produce different timings but identical prime counts.

---

**This project prioritizes correctness over security, as it processes no external input beyond command-line arguments.**
