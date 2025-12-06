\# A 2,300-Year-Old Algorithm Beats Modern AI at Code Optimization



\*How four of the world's most advanced AI systems tackled the same programming challenge — and what the results reveal about the true nature of software engineering.\*



---



\## The Challenge: Fifty Million Needles in a Billion-Number Haystack



Here's a deceptively simple task: find all the prime numbers up to one billion.



That's it. No machine learning. No neural networks. No transformers or attention mechanisms. Just pure, classical computation — the kind that would have made sense to mathematicians centuries before the first transistor was ever fabricated.



The numbers tell the story of why this is harder than it sounds. Between 1 and 1,000,000,000, there are exactly 50,847,534 prime numbers. To find them all, you need to examine — in some form or another — every single one of those billion candidates. At the scale of modern processors executing billions of operations per second, this should be trivial.



It isn't.



The naive approach — testing each number for divisibility by all smaller numbers — would take hours. Even the textbook-optimised version, testing only up to the square root, would take minutes. But the real challenge I set was more ambitious: find all fifty million primes in under one second.



This is where the ancient wisdom of Eratosthenes meets the bleeding edge of modern computing. And this is where I decided to see how today's most sophisticated AI coding assistants would fare.



---



\## When a Billion Becomes a Problem



The first trap that catches the unwary programmer is memory.



A billion boolean values — one for each candidate number — consumes one gigabyte of RAM. That's not catastrophic for a modern machine, but it's catastrophic for performance. Modern CPUs don't operate on main memory; they operate on cache. Your processor's L1 cache — the lightning-fast memory that sits closest to the execution units — holds just 32 kilobytes. When your data structure is 30,000 times larger than your cache, every memory access becomes a journey to a distant warehouse when you needed something from your desk drawer.



The second trap is arithmetic overflow. When your algorithm needs to compute p², and p can be as large as 31,623 (the square root of a billion), the result exceeds what a 32-bit integer can hold. Use `int` where you should have used `uint64\_t`, and your program doesn't crash — it silently produces garbage. Wrong prime counts. Missed primes. Phantom composites. The kind of bugs that pass cursory testing and explode in production.



The third trap is the seductive appeal of parallelism. Modern processors have multiple cores — surely we can divide the work and conquer? But the classic sieve algorithm has dependencies. You can't mark multiples of 7 until you've confirmed 7 is prime. You can't confirm 7 is prime until you've finished processing 2, 3, and 5. Naive parallelisation introduces race conditions where multiple threads write to the same memory location, corrupting results in ways that differ between runs.



Each of these traps caught at least one of our AI contestants.



---



\## The Contestants: Four Minds, One Challenge



I presented identical prompts to four of the most capable large language models available today, each equipped with extended thinking or reasoning capabilities:



\*\*Claude Opus 4.5 with Extended Thinking\*\* — Anthropic's flagship model, known for its careful reasoning and strong performance on coding benchmarks. Claude tends toward methodical, well-structured solutions and has a reputation for safety-conscious design choices.



\*\*ChatGPT 5.1 with Extended Thinking\*\* — OpenAI's latest, building on the GPT lineage that sparked the current AI revolution. GPT models have historically excelled at producing readable, well-documented code that follows conventional patterns.



\*\*Gemini 3 Pro with Thinking\*\* — Google's multimodal powerhouse, trained on what is likely the most diverse dataset of any model, including Google's vast repositories of code. Gemini brings the weight of Google's engineering culture to bear on programming tasks.



\*\*Grok 4.1 with Reasoning\*\* — xAI's offering, designed with an emphasis on reasoning capabilities and real-time information access. Grok positions itself as a more direct, less filtered assistant.



This lineup represents a reasonably fair comparison. All four models are frontier-class systems from major AI laboratories, all are equipped with chain-of-thought reasoning capabilities, and all were given exactly the same starting point: two baseline C++ implementations and a prompt asking them to do better.



The playing field, however, isn't perfectly level. Each model brings different strengths:



\*\*Claude\*\* has consistently demonstrated strong performance on complex, multi-step coding tasks, particularly those requiring careful attention to edge cases and memory safety. In benchmarks, Claude tends to produce code that compiles on the first attempt more often than competitors.



\*\*ChatGPT\*\* excels at producing idiomatic, readable code that follows best practices. Its solutions often prioritise clarity and maintainability alongside performance — the kind of code a senior engineer would approve in a code review.



\*\*Gemini\*\* brings unparalleled training data volume and Google's engineering DNA. For well-documented algorithms and patterns that appear frequently in open-source repositories, Gemini can draw on deep pattern recognition.



\*\*Grok\*\* emphasises reasoning capability and has shown strength in mathematical and logical problems. Its less filtered approach sometimes produces more creative — if occasionally unconventional — solutions.



The question I wanted to answer wasn't just "which AI writes the fastest code?" It was more nuanced: How do these different AI philosophies manifest in actual engineering output? And can any of them match what a skilled human programmer, armed with decades of collective optimisation wisdom, would produce?



---



\## The Rules of Engagement



Each model received the same prompt, styled with the kind of context-setting that tends to elicit focused technical responses:



> \*"...can you improve on the code in either of the two attached `.cpp`s - they both benchmark pretty evenly, alternating as to which comes out fastest. - i'm interested in the fastest \[priority] - shortest code \[in lines total] - most elegant \[this is an exercise purely to see how cleverly we can code prime sort algos] can you do better than either of these? \[i don't mind longer than c-primes.cpp if it's faster, or same speed if it's less code/even more elegant than c-primes-fast `.cpp`]..."\*



Along with the theatrical framing came concrete criteria:



1\. \*\*Speed\*\* — fastest execution time wins

2\. \*\*Compactness\*\* — fewer lines of code is better

3\. \*\*Elegance\*\* — readability and idiom usage matter



Each criterion was worth 10 points, for a maximum score of 30.



A final restriction to try and keep it fair - I only accepted code that would compile according to this particular command:


```
Normalized compile command all had to adhere to, to keep things fair and consistent;

g++ -O2 -std=c++20 -Wall -Wextra -pedantic -o c-primes-xxxxx-xxxxx-1e9.exe c-primes-xxxxx-xxxxx-1e9.cpp

```



Gemini's cheat \[insisted I use this to compile ad wouldn't give code that compiled with the 'normal' command for the last code block]:

```

g++ -O3 -march=native -fopenmp -std=c++20 c-primes-xxxxx-xxxxx.cpp -o c-primes-xxxxx-xxxxx.exe

```



note the `fopemp` which he just threw a hissy fit and insisted had to be used and wouldn't have it any other way...



I also attached two baseline implementations: a textbook Sieve of Eratosthenes, and a more optimised version using bit-packing and odd-number-only representation. The challenge was to improve on both.



What happened next revealed as much about the nature of software engineering as it did about the capabilities of these AI systems.



---



\## Round One: The 500,000 Warm-Up



Before tackling a billion, I started with a gentler target: all primes up to 500,000.



At this scale, the memory problem largely disappears. The entire sieve fits comfortably in L2 cache. There are no overflow concerns with 32-bit integers. This was a test of pure algorithmic thinking and code quality.



\*\*Claude\*\* delivered a solution in 18 lines of dense, intrinsic-heavy C++. It compiled without errors on the first attempt. The code used `\_\_builtin\_ctzll` for efficient bit scanning, the Kernighan trick for extracting set bits, and aggressive expression compression. It was fast and compact — but not pretty.



\*\*ChatGPT\*\* produced 45 lines of elegant, readable code. Named helper macros. Explicit loop counters. Clear variable naming. It had a single minor issue on first submission — a missing flag in the compile command — but worked perfectly once corrected. The code was a pleasure to read, the kind you'd want in a codebase that humans need to maintain.



\*\*Gemini\*\* and \*\*Grok\*\* both required multiple iterations. Compilation errors. Runtime failures. Off-by-one bugs. Each needed several rounds of correction before producing working code. Their final solutions functioned, but bore the marks of iterative debugging rather than coherent design.



The benchmark results for Round One (10 runs, 10 repetitions each):



| Model    | Avg Time (ms) |   Lines   |     First-Attempt Success    |

|-------|------------|-------|-----------------------|

│Claude  │            24.90   	  │        18	       │   Yes				 	          │

│Gpetey  │            25.20   	  │        44	       │   Nearly (compile flag)	│

│Grok        │            25.57   	  │        51	       │   No (3 iterations) 	       	│

│Gemini  │            29.66   	  │        44        │   No (4 iterations)		│



Claude won on speed and compactness. ChatGPT won on elegance. The performance gap between the top two was just 1.4% — essentially a tie. But the gap in code quality and first-attempt success rate told a more interesting story.



---



\## Round Two: The Billion-Prime Gauntlet



Scaling to a billion changes everything.



At n = 1,000,000,000, naive implementations hit the memory wall. The integer overflow trap springs. The cache hierarchy becomes the dominant performance factor. This is where real algorithmic sophistication separates from textbook knowledge.



All four models were asked to adapt their approaches. All four needed to discover — or already know — that segmented sieving was essential.



\*\*Claude\*\* immediately identified the need for segmentation, cache-line optimisation, and 64-bit arithmetic. The code compiled cleanly, produced correct results, and ran in approximately 2.1 seconds on my test machine.



\*\*ChatGPT\*\* also transitioned to segmented sieving, with its characteristic attention to code clarity. A small type-mismatch issue required one fix, but the corrected code ran correctly in approximately 2.2 seconds.



\*\*Gemini\*\* and \*\*Grok\*\* struggled more significantly. Both produced initial implementations with integer overflow bugs — the p² calculation silently wrapped, producing incorrect results. Both required multiple iterations to achieve correct output. Their final timings were in the 2.4-2.5 second range.



The pattern from Round One repeated at larger scale: Claude and ChatGPT led on both performance and code quality, with Claude maintaining a slight speed edge and ChatGPT maintaining superior readability.



---



\## Breaking the One-Second Barrier



None of the initial LLM implementations broke the one-second barrier.



To achieve that target, I needed to layer additional optimisations on top of the base algorithms: SIMD vectorisation, multi-threaded parallelism, and careful cache-aware segment sizing.



The final "ultimate" implementation — combining AVX2 SIMD instructions with multi-threaded work-stealing — achieved 224 milliseconds on a 4-core machine. That's finding all 50,847,534 primes in less than a quarter of a second, processing over 4 billion integers per second.



But here's the crucial insight: \*none of that speed came from algorithmic cleverness\*.



The core algorithm was still the Sieve of Eratosthenes — unchanged in its fundamental logic since a Greek mathematician invented it in the third century BCE. What made it fast was:



\- \*\*Parallelism\*\* — distributing work across cores

\- \*\*SIMD\*\* — processing multiple data elements per instruction

\- \*\*Cache optimisation\*\* — fitting working sets in L1

\- \*\*Bit manipulation tricks\*\* — Kernighan's algorithm, `ctzll` intrinsics



These are engineering techniques, not algorithmic innovations. They're the kind of thing a skilled human programmer learns from experience, from reading architecture manuals, from profiling real code on real hardware.



The LLMs could implement the algorithm correctly. The best of them could even apply some of these optimisations. But none of them spontaneously invented a better algorithm — because for this problem, there isn't one.



---



\## # Prime Sieve Techniques: A Complete Guide



From ancient algorithms to modern SIMD-accelerated parallel implementations, this guide covers every major technique for generating prime numbers efficiently.



---



\## Table of Contents



1\. \[The Fundamental Problem](#the-fundamental-problem)

2\. \[Trial Division (Naive Approach)](#trial-division)

3\. \[Sieve of Eratosthenes (The Classic)](#sieve-of-eratosthenes)

4\. \[Odd-Only Sieve](#odd-only-sieve)

5\. \[Bit-Packing](#bit-packing)

6\. \[Wheel Factorization](#wheel-factorization)

7\. \[Segmented Sieve](#segmented-sieve)

8\. \[Kernighan Bit-Clear Extraction](#kernighan-bit-clear-extraction)

9\. \[Loop Unrolling](#loop-unrolling)

10\. \[SIMD Vectorization](#simd-vectorization)

11\. \[Parallel Sieving](#parallel-sieving)

12\. \[Sieve of Atkin](#sieve-of-atkin)

13\. \[Sieve of Sundaram](#sieve-of-sundaram)

14\. \[Comparison Table](#comparison-table)



---



\## The Fundamental Problem



\*\*Goal:\*\* Find all prime numbers up to a given limit n.



\*\*Definition:\*\* A prime number is a natural number greater than 1 that has no positive divisors other than 1 and itself.



\*\*The Challenge:\*\* As n grows, the computational and memory requirements explode. For n = 1 billion:

\- There are 50,847,534 primes to find

\- Naive approaches would take hours

\- Memory-naive approaches need 1GB+ RAM



\*\*The Solution:\*\* Clever algorithmic techniques that trade different resources (time, memory, complexity) against each other.



---



\## Trial Division



\*\*The simplest approach — and the slowest.\*\*



\### How It Works



For each candidate number, test if any smaller number divides it evenly:



```cpp

bool is\_prime(int n) {

&nbsp;   if (n < 2) return false;

&nbsp;   for (int i = 2; i \* i <= n; i++) {

&nbsp;       if (n % i == 0) return false;

&nbsp;   }

&nbsp;   return true;

}

```



\### Complexity



\- \*\*Time:\*\* O(n√n) — for each of n numbers, test up to √n divisors

\- \*\*Space:\*\* O(1) — no extra storage needed



\### When to Use



\- Educational purposes

\- Checking if a single number is prime

\- Never for bulk generation



\### The Problem



At n = 1,000,000, this takes seconds. At n = 1,000,000,000, it would take hours.



---



\## Sieve of Eratosthenes



\*\*The 2,300-year-old algorithm that still forms the basis of modern implementations.\*\*



\### History



Invented by Eratosthenes of Cyrene (~276-194 BCE), a Greek mathematician who also calculated the Earth's circumference with remarkable accuracy.



\### How It Works



1\. Create a list of all numbers from 2 to n

2\. Start with the first unmarked number (2)

3\. Mark all its multiples as composite

4\. Move to the next unmarked number

5\. Repeat until you've processed all numbers up to √n



```cpp

vector<bool> sieve(int n) {

&nbsp;   vector<bool> is\_prime(n + 1, true);

&nbsp;   is\_prime\[0] = is\_prime\[1] = false;

&nbsp;   

&nbsp;   for (int p = 2; p \* p <= n; p++) {

&nbsp;       if (is\_prime\[p]) {

&nbsp;           for (int i = p \* p; i <= n; i += p) {

&nbsp;               is\_prime\[i] = false;

&nbsp;           }

&nbsp;       }

&nbsp;   }

&nbsp;   return is\_prime;

}

```



\### Key Insight: Start at p²



When marking multiples of prime p, we start at p² rather than 2p. Why?



All multiples of p smaller than p² have already been marked by smaller primes:

\- 2p was marked when processing prime 2

\- 3p was marked when processing prime 3

\- etc.



\### Complexity



\- \*\*Time:\*\* O(n log log n) — remarkably efficient!

\- \*\*Space:\*\* O(n) — one boolean per number



\### Why O(n log log n)?



The sum of reciprocals of primes up to n is approximately ln(ln(n)). Each prime p marks n/p composites. The total work is:



```

n/2 + n/3 + n/5 + n/7 + ... ≈ n × ln(ln(n))

```



This is almost linear — only slightly worse than O(n).



---



\## Odd-Only Sieve



\*\*First optimization: skip even numbers entirely.\*\*



\### The Insight



Except for 2, all primes are odd. Why store or process even numbers at all?



\### Implementation



Map index i to odd number 2i + 1:

\- Index 0 → 1 (not prime)

\- Index 1 → 3 (prime)

\- Index 2 → 5 (prime)

\- Index 3 → 7 (prime)

\- Index 4 → 9 (composite: 3²)



```cpp

// Only odd numbers: index i represents number 2i + 1

vector<bool> sieve\_odd(int n) {

&nbsp;   int size = n / 2 + 1;

&nbsp;   vector<bool> is\_prime(size, true);

&nbsp;   is\_prime\[0] = false;  // 1 is not prime

&nbsp;   

&nbsp;   for (int i = 1; 2\*i\*(i+1) < size; i++) {

&nbsp;       if (is\_prime\[i]) {

&nbsp;           int p = 2\*i + 1;

&nbsp;           // Mark odd multiples of p starting at p²

&nbsp;           for (int j = 2\*i\*(i+1); j < size; j += p) {

&nbsp;               is\_prime\[j] = false;

&nbsp;           }

&nbsp;       }

&nbsp;   }

&nbsp;   return is\_prime;

}

```



\### Benefits



\- \*\*Memory:\*\* 50% reduction (n/2 bits instead of n)

\- \*\*Time:\*\* ~50% faster (half the iterations)

\- \*\*Cache:\*\* Better utilization



\### The Math Behind j = 2\*i\*(i+1)



If index i represents prime p = 2i + 1, then p² = (2i + 1)² = 4i² + 4i + 1.



The index of p² in our odd-only array is (p² - 1) / 2 = (4i² + 4i) / 2 = 2i(i + 1).



---



\## Bit-Packing



\*\*Pack 64 candidates into a single 64-bit word.\*\*



\### The Problem with vector<bool>



C++ `vector<bool>` is already bit-packed, but:

\- Access patterns are not cache-optimal

\- No control over memory layout

\- Overhead from bounds checking



\### Manual Bit-Packing



Store 64 numbers per `uint64\_t`:



```cpp

// Bit manipulation macros

\#define SET\_BIT(arr, i)   (arr\[(i) >> 6] |=  (1ULL << ((i) \& 63)))

\#define CLR\_BIT(arr, i)   (arr\[(i) >> 6] \&= ~(1ULL << ((i) \& 63)))

\#define TST\_BIT(arr, i)   (arr\[(i) >> 6] \&   (1ULL << ((i) \& 63)))

```



\### Understanding the Bit Math



\- `(i) >> 6` — Divide by 64 to get the word index

\- `(i) \& 63` — Modulo 64 to get the bit position within the word

\- `1ULL << bit` — Create a mask with only that bit set



\### Combined with Odd-Only



When combining bit-packing with odd-only representation:

\- Index i represents odd number 2i + 1

\- Word w contains indices 64w through 64w + 63

\- These represent odd numbers 128w + 1 through 128w + 127



\### Benefits



\- \*\*Memory:\*\* 8 bytes stores 64 candidates (vs. 64 bytes for bool array)

\- \*\*Cache:\*\* Entire L1 cache can hold 256K candidates

\- \*\*Vectorization:\*\* Natural 64-bit operations



---



\## Wheel Factorization



\*\*Skip multiples of small primes beyond just 2.\*\*



\### The Concept



If we skip multiples of 2, we eliminate 50% of candidates. What if we also skip multiples of 3? And 5?



\### Wheel-6 (Skip 2, 3)



Only numbers of the form 6k ± 1 can be prime (except 2 and 3):

\- 6k + 0 = divisible by 6

\- 6k + 1 = potential prime ✓

\- 6k + 2 = divisible by 2

\- 6k + 3 = divisible by 3

\- 6k + 4 = divisible by 2

\- 6k + 5 = potential prime ✓ (same as 6k - 1)



\*\*Density:\*\* 2/6 = 33% of numbers are candidates (vs. 50% for odd-only)



\### Wheel-30 (Skip 2, 3, 5)



Only 8 residues modulo 30 can be prime:

```

1, 7, 11, 13, 17, 19, 23, 29

```



\*\*Density:\*\* 8/30 = 26.7% of numbers are candidates



\### Wheel-210 (Skip 2, 3, 5, 7)



48 residues modulo 210.



\*\*Density:\*\* 48/210 = 22.9% of numbers are candidates



\### Diminishing Returns



| Wheel | Period | Candidates 	| Density | Improvement |

|-------|--------|---------|-------|------------|

| 2 		| 2 		   | 1 			| 50.0% 	  | baseline 		|

| 6 		| 6 		   | 2 			| 33.3% 	  | 1.5x 			|

| 30 		| 30 		   | 8 			| 26.7% 	  | 1.25x			|

| 210 		| 210 		   | 48 			| 22.9% 	  | 1.17x 			|

| 2310 	| 2310 	   | 480 		| 20.8% 	  | 1.10x 			|



The complexity of implementing larger wheels grows faster than the benefit.



\### Implementation Challenge



Wheel factorization requires:

\- Mapping between wheel positions and actual numbers

\- Handling the irregular gaps between candidates

\- More complex loop structures



Often the overhead exceeds the benefit, especially at smaller n.



---



\## Segmented Sieve



\*\*The key to sieving billions: process in cache-sized chunks.\*\*



\### The Memory Problem



For n = 1,000,000,000:

\- Basic sieve: 1 billion booleans = 1 GB

\- Odd-only: 500 million bits = 62.5 MB

\- Still too large for CPU cache!



\### The Cache Hierarchy



| Cache | Size      | Latency 		|

|------|------|------------|

| L1 	     | 32 KB    | ~4 cycles 	|

| L2 	     | 256 KB | ~12 cycles 	|

| L3 	     | 8 MB 	  | ~40 cycles 	|

| RAM 	     | 16+ GB | ~200 cycles 	|



Every cache miss costs 50x the time of a cache hit!



\### The Solution: Segmentation



1\. Find all primes up to √n (the "base primes")

2\. Process the range \[√n, n] in segments that fit in L1 cache

3\. For each segment, mark composites using base primes

4\. Extract primes from the segment



```

\[2, √n] → Base primes (small sieve)

&nbsp;   ↓

\[√n+1, √n+S] → Segment 1

\[√n+S+1, √n+2S] → Segment 2

&nbsp;   ...

\[n-S+1, n] → Final segment

```



\### Segment Size Selection



\*\*Optimal segment size:\*\* Fits in L1 cache with room for base primes.



\- L1 = 32 KB

\- Base primes for n=10⁹: ~3,400 primes × 4 bytes = 13.6 KB

\- Remaining: ~18 KB for segment

\- Segment size: ~16 KB = 128K odd numbers



\### Memory Complexity



\- \*\*Base primes:\*\* O(√n / ln(√n)) ≈ O(√n)

\- \*\*Segment buffer:\*\* O(segment\_size) — constant!

\- \*\*Total:\*\* O(√n) instead of O(n)



For n = 10⁹: ~30 KB instead of 62.5 MB — a 2000x reduction!



---



\## Kernighan Bit-Clear Extraction



\*\*Extract primes by visiting only set bits.\*\*



\### The Naive Approach



```cpp

// O(n) - visits every bit

for (int i = 0; i < n; i++) {

&nbsp;   if (is\_prime\[i]) {

&nbsp;       primes.push\_back(i);

&nbsp;   }

}

```



\### The Kernighan Trick



Brian Kernighan (co-creator of C) discovered a beautiful identity:



```cpp

x \& (x - 1)  // Clears the lowest set bit of x

```



\*\*Why it works:\*\*

\- Subtracting 1 flips all bits from the lowest set bit rightward

\- AND-ing with the original clears the lowest set bit



Example: x = 01011000

\- x - 1 = 01010111

\- x \& (x-1) = 01010000



\### Applied to Prime Extraction



```cpp

for (size\_t i = 0; i < words; i++) {

&nbsp;   uint64\_t w = bits\[i];

&nbsp;   while (w) {

&nbsp;       int pos = \_\_builtin\_ctzll(w);  // Count trailing zeros

&nbsp;       int prime = (i \* 128) + (pos \* 2) + 1;

&nbsp;       primes.push\_back(prime);

&nbsp;       w \&= w - 1;  // Clear lowest bit, move to next

&nbsp;   }

}

```



\### Complexity



\- \*\*Naive:\*\* O(n) — visit every candidate

\- \*\*Kernighan:\*\* O(π(n)) — visit only primes



For n = 10⁹: 50 million iterations instead of 500 million — 10x fewer!



\### The ctzll Intrinsic



`\_\_builtin\_ctzll(x)` counts trailing zeros in x (GCC/Clang).



On x86, this compiles to a single `TZCNT` or `BSF` instruction — O(1) time.



Windows equivalent: `\_BitScanForward64()`.



---



\## Loop Unrolling



\*\*Reduce loop overhead by processing multiple elements per iteration.\*\*



\### The Problem



Loops have overhead:

\- Increment counter

\- Compare to limit

\- Branch prediction



For tight inner loops, this overhead is significant.



\### Manual Unrolling



```cpp

// Before: 1 operation per iteration

for (int j = start; j < limit; j += step) {

&nbsp;   clear\_bit(j);

}



// After: 4 operations per iteration

for (; j + 3\*step < limit; j += 4\*step) {

&nbsp;   clear\_bit(j);

&nbsp;   clear\_bit(j + step);

&nbsp;   clear\_bit(j + 2\*step);

&nbsp;   clear\_bit(j + 3\*step);

}

// Handle remainder

for (; j < limit; j += step) {

&nbsp;   clear\_bit(j);

}

```



\### Benefits



\- \*\*Fewer branches:\*\* 4x fewer loop iterations

\- \*\*Better pipelining:\*\* CPU can execute independent operations in parallel

\- \*\*Instruction-level parallelism:\*\* Multiple operations in flight



\### Optimal Unroll Factor



\- Too little: Overhead remains

\- Too much: Instruction cache pressure, code bloat



Typical sweet spot: 4-8x unrolling for simple operations.



---



\## SIMD Vectorization



\*\*Process multiple data elements with single instructions.\*\*



\### What is SIMD?



\*\*S\*\*ingle \*\*I\*\*nstruction, \*\*M\*\*ultiple \*\*D\*\*ata.



Modern CPUs have vector registers that can process multiple values simultaneously:



| Technology | Register Size | 64-bit Values  |

|-----------|-------------|-------------|

| SSE2 		    | 128 bits 		     | 2 					|

| AVX2 		    | 256 bits 		     | 4 					|

| AVX-512 	    | 512 bits 		     | 8 					|



\### Where SIMD Helps in Sieving



\*\*Memory initialization:\*\*

```cpp

// Scalar: 1 word per instruction

for (int i = 0; i < words; i++)

&nbsp;   arr\[i] = ~0ULL;



// AVX2: 4 words per instruction

\_\_m256i ones = \_mm256\_set1\_epi64x(-1LL);

for (int i = 0; i < words; i += 4)

&nbsp;   \_mm256\_store\_si256((\_\_m256i\*)\&arr\[i], ones);

```



\*\*Population counting:\*\*

```cpp

// AVX-512 has native VPOPCNTDQ

// AVX2 requires lookup tables or multiple instructions

```



\### Where SIMD Doesn't Help



\*\*Random bit clearing:\*\* Sieve marking accesses memory at irregular intervals determined by each prime. SIMD works best on contiguous, predictable access patterns.



The irregular pattern p, 2p, 3p, 4p... doesn't vectorize well because:

\- Different primes have different strides

\- Accesses span different cache lines

\- Dependencies between iterations



\### Practical SIMD Strategy



1\. Use SIMD for bulk memory operations (fill, copy)

2\. Use scalar code for marking (irregular access)

3\. Use SIMD-friendly popcount for counting

4\. Let the compiler auto-vectorize where it can



---



\## Parallel Sieving



\*\*Distribute work across multiple CPU cores.\*\*



\### The Challenge



The basic sieve has dependencies — we need prime p before marking its multiples.



But segmented sieving is embarrassingly parallel! Each segment can be processed independently once we have base primes.



\### Work Distribution Strategies



\*\*Static partitioning:\*\*

\- Divide range into equal chunks per thread

\- Simple, but load imbalance if chunks have different prime densities



\*\*Dynamic work-stealing:\*\*

```cpp

std::atomic<int> next\_segment{0};



auto worker = \[\&]() {

&nbsp;   while (true) {

&nbsp;       int seg = next\_segment.fetch\_add(1);

&nbsp;       if (seg >= total\_segments) break;

&nbsp;       process\_segment(seg);

&nbsp;   }

};

```



This automatically balances load — fast threads take more segments.



\### What Parallelizes



✅ Segment processing — each segment is independent

✅ Prime counting — sum per-thread counts

✅ Memory initialization — parallel fills



\### What Doesn't Parallelize



❌ Base prime generation — small enough to not matter

❌ Ordered prime collection — need synchronization

❌ Finding "last N primes" — requires final pass



\### Speedup Expectations



\*\*Ideal:\*\* Linear with core count (8 cores = 8x faster)



\*\*Reality:\*\* 

\- Memory bandwidth limits scaling

\- Cache contention between threads

\- Synchronization overhead



Typical achieved speedup: 3-6x on 8 cores.



---



\## Sieve of Atkin



\*\*A modern alternative with better theoretical complexity.\*\*



\### Background



Developed by A.O.L. Atkin and Daniel J. Bernstein in 2003.



\### How It Works



Instead of marking composites, it uses quadratic forms to identify primes:



1\. Numbers with odd count of solutions to 4x² + y² = n (mod 12 ∈ {1, 5})

2\. Numbers with odd count of solutions to 3x² + y² = n (mod 12 = 7)

3\. Numbers with odd count of solutions to 3x² - y² = n (mod 12 = 11, x > y)



Then eliminate squares of primes.



\### Complexity



\- \*\*Time:\*\* O(n / log log n) — theoretically better than Eratosthenes!

\- \*\*Space:\*\* O(n)



\### In Practice



Despite better theoretical complexity:

\- Constant factors are higher

\- More complex to implement

\- Only faster for very large n (>10¹⁰)

\- Harder to optimize with SIMD/parallelism



For most practical purposes, optimized Eratosthenes wins.



---



\## Sieve of Sundaram



\*\*An elegant algorithm that naturally produces odd primes.\*\*



\### How It Works



1\. Create array for numbers 1 to n

2\. Mark all numbers of form i + j + 2ij where 1 ≤ i ≤ j and i + j + 2ij ≤ n

3\. Remaining numbers k give primes 2k + 1



\### The Math



If k is not marked, then 2k + 1 is prime.



The marked numbers represent exactly those k where 2k + 1 is an odd composite.



\### Complexity



\- \*\*Time:\*\* O(n log n) — worse than Eratosthenes

\- \*\*Space:\*\* O(n)



\### When to Use



Mostly of theoretical interest. The natural odd-number output is elegant, but performance doesn't match optimized Eratosthenes.



---



\## Comparison Table



| Technique 			 | Time 			   	  | Space             | Practical Speed | Complexity  |

|------------------|---------------|----------|---------------|-----------|

| Trial Division 		 | O(n√n) | O(1)   	  | Very Slow   | Trivial 			  |				     |

| Basic Eratosthenes | O(n log log n)  	  | O(n) 		  | Slow 				  | Simple 	     |

| Odd-Only 				 | O(n log log n)  	  | O(n/2) 	  | Medium 			  | Simple 	     |

| Bit-Packed 			 | O(n log log n)  	  | O(n/16) 	  | Fast 				  | Medium 	     |

| Wheel-30 				 | O(n log log n)  	  | O(n/30) 	  | Fast 				  | Complex 	     |

| Segmented 			 | O(n log log n)  	  | O(√n) 	  	   | Very Fast 		  | Medium 	     |

| + Kernighan 			 | O(n log log n) 	  | O(√n) 		   | Very Fast 		  | Medium 	     |

| + SIMD 					 | O(n log log n) 	  | O(√n) 	 	   | Faster 			  | Complex 	     |

| + Parallel 			 | O(n log log n / p) | O(√n × p) 	    | Fastest 			  | Complex 	     |



---



\## Putting It All Together



The ultimate modern prime sieve combines:



1\. \*\*Segmented approach\*\* — O(√n) memory, cache-friendly

2\. \*\*Odd-only representation\*\* — 50% memory/time savings

3\. \*\*64-bit packed storage\*\* — Cache-optimal, SIMD-friendly

4\. \*\*Kernighan extraction\*\* — O(primes) not O(candidates)

5\. \*\*Loop unrolling\*\* — Reduced branch overhead

6\. \*\*SIMD initialization\*\* — Fast segment setup

7\. \*\*Parallel processing\*\* — Linear scaling with cores



This combination achieves:

\- \*\*1 billion primes in ~250ms\*\* on modern hardware

\- \*\*30 KB memory\*\* instead of 125 MB

\- \*\*4+ billion integers/second\*\* throughput



The 2,300-year-old algorithm of Eratosthenes, enhanced with 50 years of computer science optimization techniques, remains unbeaten for practical prime generation.



---



\## The Final Results: Crowning the Champion



After all rounds and all optimisations, the final standings emerged:



\# Prime-Shootout: Final Benchmark Results



All benchmarks: 10 runs × 10 repeats per run

Platform: Windows 11, AMD/Intel x64

Compiler: GCC 13.2, -O3 -march=native -std=c++17



---



\## Round 1: n = 500,000 (41,538 primes)



```

┌─────────────────────────┬──────────────┬─────────────┬──────┐

│ Implementation            				 │        Mean Avg (ms)	    │       Std Dev (ms)	   │   Runs     │

├─────────────────────────┼──────────────┼─────────────┼──────┤

│ c-primes-claude         				 │        24.90 	 		    │        4.05 			   │   10 	       │

│ c-primes-gpetey         				 │        25.20 	 		    │        6.65 			   │   10 	       │

│ c-primes-grok               				 │        25.57 	 		    │        3.99 			   │   10 	       │

│ c-primes-gemini         				 │        29.66 	 		    │       10.42   		   │   10 	       │

├─────────────────────────┼──────────────┼─────────────┼──────┤

│ r-primes-fast           				 │        31.45 			    │        5.91 			   │   10 	       │

│ r-primes                					 │        37.76 			    │       19.46 			   │   10 	       │

├─────────────────────────┼──────────────┼─────────────┼──────┤

│ c-primes-fast (baseline)		 │        37.96 			    │       23.51 			   │   10 	       │

│ c-primes (baseline)     			 │        40.55 			    │       17.10 			   │   10 	       │

├─────────────────────────┼──────────────┼─────────────┼──────┤

│ p-primes-smart-50k      			 │        2502.16 		    │       303.85 		   │   10 	       │

│ p-primes-smart-500      			 │        2524.25 		    │       287.17 		   │   10 	       │

│ p-primes-smart-500k     			 │        3125.96 		    │       325.40 		   │   10 	       │

└─────────────────────────┴──────────────┴─────────────┴──────┘

```



\### Round 1 Winner: Claude (24.90 ms)



\*\*LLM Rankings:\*\*

1\. Claude — 24.90 ms (fastest, lowest variance)

2\. GPetey — 25.20 ms (+1.2%)

3\. Grok — 25.57 ms (+2.7%)

4\. Gemini — 29.66 ms (+19.1%)



---



\## Round 2: n = 1,000,000,000 (50,847,534 primes)



\### Non-Segmented Implementations



```

┌──────────────────────────┬──────────────┬─────────────┬─────────────┬─────────────┐

│ Implementation           				     │   Avg (ms)     		       │   Min (ms)   		      │   Max (ms)    		     │   StdDev (ms) 	    │

├──────────────────────────┼──────────────┼─────────────┼─────────────┼─────────────┤

│ r-primes-fast-1e9        			    │     2,265.82 		       │    2,156.45 		      │    2,579.73 		     │      115.03 		    │

│ c-primes-fast-claude-1e9		    │     3,170.10 		       │    3,085.33 		      │    3,300.26 		     │       52.59 		    │

│ c-primes-fast-1e9        			    │     3,719.54 		       │    3,612.04 		      │    4,013.30 		     │      115.38 		    │

│ c-primes-fast-gpetey-1e9 		    │     3,757.17 		       │    3,633.55 		      │    3,932.95 		     │       90.78 		    │

├──────────────────────────┼──────────────┼─────────────┼─────────────┼─────────────┤

│ c-primes-1e9 (baseline)  		    │    16,737.74 		       │   15,433.22 		      │   17,458.00 		     │      525.27 		    │

│ r-primes-1e9 (baseline)  		    │    19,458.01 		       │   14,370.34 		      │   20,654.37 		     │      1,716.09 		    │

└──────────────────────────┴──────────────┴─────────────┴─────────────┴─────────────┘

```



\### Segmented Implementations



```

┌─────────────────────────────┬──────────────┬─────────────┬─────────────┬─────────────┐

│ Implementation              				     │     Avg (ms)    		        │     Min (ms)   	       │     Max (ms)                  │      StdDev (ms) 	     │

├─────────────────────────────┼──────────────┼─────────────┼─────────────┼─────────────┤

│ c-primes-fast-gpetey-seg-1e9		     │       928.52 		        │      897.32 		       │      983.53 		      │       22.15 		     │	

│ c-primes-fast-claude-seg-1e9		     │       1,143.79 		        │      1,129.32 		       │      1,167.19 		      │       11.76 		     │

└─────────────────────────────┴──────────────┴─────────────┴─────────────┴─────────────┘

```



\### Round 2 Winner: GPetey Segmented (928.52 ms)



\*\*LLM Rankings (Segmented):\*\*

1\. GPetey — 928.52 ms (fastest)

2\. Claude — 1,143.79 ms (+23.2%, but lowest variance)



---



\## Round Aux: Specialised Implementations (n = 1,000,000,000)



```

┌──────────────────────────┬──────────────┬─────────────┬──────┐

│ Implementation           				    │       Mean Avg (ms)       │        Std Dev (ms)     │    Runs 	 │

├──────────────────────────┼──────────────┼─────────────┼──────┤

│ c-primes-the-beast-1e9   		     │        12.39 		       │        0.30 		      │   10 		 │

│ c-primes-bitpacked-1e9   		     │        12.60 		       │        0.72 		      │   10 		 │

│ c-primes-parallel-1e9    		     │        12.65 		       │        0.87 		      │   10		 │

│ r-primes-wheel-1e9       			     │        12.99 		       │        1.01 		      │   10 		 │

└──────────────────────────┴──────────────┴─────────────┴──────┘

```



\*Note: These implementations output prime count only, not full prime list.\*



---



\## Round SIMD: Vectorised Implementations (n = 1,000,000,000)



```

┌──────────────────────────┬──────────────┬─────────────┬──────┐

│ Implementation           				     │      Mean Avg (ms)       │      Std Dev (ms)	      │    Runs 	 │

├──────────────────────────┼──────────────┼─────────────┼──────┤

│ c-primes-parallel-1e9    		     │       719.43 		       │       16.27 		      │   10 		 │

│ c-primes-simd-1e9        			     │       1,448.90 		       │       17.12 		      │   10 		 │

└──────────────────────────┴──────────────┴─────────────┴──────┘

```



\*\*Key Insight:\*\* Multi-threading (719 ms) beats SIMD-only (1,449 ms) by 2x.

Combined SIMD + Parallel achieves best results.



---



\## Summary: Final Standings



\### By Implementation Type



```

┌────────────────────────────────┬──────────────┬───────────────────┐

│ Category                       						      │    Best Time   	        │   Winner            			        │

├────────────────────────────────┼──────────────┼───────────────────┤

│ Round 1 (n=500K)               					      │     24.90 ms 		        │  Claude            			        │

│ Round 2 Non-Seg (n=1B)         				      │    2,265.82 ms 	        │  Rust Fast         			        │

│ Round 2 Segmented (n=1B)       				      │    928.52 ms 		        │  GPetey           			        │

│ Parallel Only (n=1B)           				      │    719.43 ms 		        │  Multi-threaded   		        │

│ SIMD Only (n=1B)               					      │    1,448.90 ms 	        │ AVX2              				        │

│ Specialised Count-Only (n=1B)  			      │     12.39 ms 		        │ The Beast         			        │

└────────────────────────────────┴──────────────┴───────────────────┘

```



\### LLM Head-to-Head



```

┌─────────┬────────────┬────────────┬─────────────┬────────────────────┐

│ LLM     		     │ Round 1    	 	 │ Round 2    		      │ First-Try   	     │ Code Quality       		         │

│         		     │ (500K)     		 │ (1B Seg)   	      │ Success     		     │                    					         │

├─────────┼────────────┼────────────┼─────────────┼────────────────────┤

│ Claude  	     │ 24.90 ms   		 │ 1,143.79ms 	      │ Yes         			     │ Compact, efficient 	         │

│ GPetey  	     │ 25.20 ms   		 │   928.52ms 	      │ Nearly      		     │ Elegant, readable  		         │

│ Grok    	     │ 25.57 ms   		 │     —      		      │ No (4 iter) 	     │ Functional         			         │

│ Gemini  	     │ 29.66 ms   	 	 │     —      		      │ No (3 iter) 	     │ Verbose            			         │

└─────────┴────────────┴────────────┴─────────────┴────────────────────┘

```



\### Final Scores (30 points possible)



```

┌─────────┬───────┬──────────┬─────────┬───────┐

│ LLM     		     │ Speed 	   │ Compact  	 │ Elegant 	       │ Total        │

├─────────┼───────┼──────────┼─────────┼───────┤

│ Claude  	     │  10   		   │    10    		 │    0    		       │  20   	     │

│ GPetey  	     │   5   		   │     0    			 │   10    		       │  15   	     │

│ Grok    	     │   3   		   │     0    			 │    0    		       │   3   		     │

│ Gemini  	     │   0   		   │     0    			 │    0    		       │   0   		     │

└─────────┴───────┴──────────┴─────────┴───────┘

```



---



\## Key Findings



1\. \*\*Claude\*\* produced the most compact code (18 lines) with zero compilation errors

2\. \*\*GPetey\*\* produced the most elegant/readable code with fastest segmented performance

3\. \*\*Grok\*\* and \*\*Gemini\*\* required multiple iterations to achieve working code

4\. \*\*Parallelism\*\* provides 2x speedup over SIMD alone at this scale

5\. \*\*Segmentation\*\* is essential for billion-scale — 4x faster than non-segmented

6\. All LLMs used the \*\*Sieve of Eratosthenes\*\* — no novel algorithms emerged



---



\*Benchmarks conducted December 2025\*

\*Full source code: github.com/whisprer/prime-shootout\*



---



\## What This Really Tells Us About AI and Code



The most striking result isn't the timing differences — it's the pattern of success and failure.



\*\*Claude made zero errors.\*\* Every piece of code it produced compiled and ran correctly on the first attempt. This isn't a coincidence; it reflects a design philosophy that prioritises correctness and careful reasoning.



\*\*ChatGPT made exactly one error\*\* — and it was a trivial compile flag issue, not a logic bug. Its code was beautiful: well-structured, clearly named, following conventions that any C++ programmer would recognise and appreciate.



\*\*Gemini and Grok produced substantially messier results.\*\* Multiple iterations were required to achieve working code. The final implementations bore the scars of iterative debugging — inconsistent naming, vestigial code paths, less coherent structure.



This matters more than raw speed.



In real software engineering, a solution that works on the first attempt is worth more than a marginally faster solution that requires three rounds of debugging. The time spent fixing bugs, understanding failure modes, and iterating toward correctness is time not spent on other problems.



Moreover, code doesn't exist in isolation. It lives in codebases maintained by teams over years. \*\*ChatGPT's elegant, readable code\*\* — despite being marginally slower than Claude's — might be the better engineering choice for a production system, because it's the code that future maintainers will understand and modify without introducing new bugs.



---



\## The Deeper Lesson: Algorithms Beat Cleverness



Perhaps the most humbling result of this experiment is what \*didn't\* happen.



No LLM invented a new algorithm. No AI discovered a novel approach that outperformed the 2,300-year-old Sieve of Eratosthenes. Despite being trained on essentially all of human programming knowledge — millions of repositories, billions of lines of code, decades of computer science research — the best these systems could do was implement existing techniques well.



The performance gains that mattered most came from:



1\. \*\*Hardware exploitation\*\* — parallelism, SIMD, cache awareness

2\. \*\*Implementation details\*\* — bit manipulation, loop unrolling, memory layout

3\. \*\*Avoiding mistakes\*\* — correct types, proper bounds, thread safety



None of these require creativity or insight. They require \*knowledge\* — deep, specific knowledge of how modern computers actually work.



And here's the thing: \*\*humans have this knowledge too.\*\* The segmented sieve is well-documented. The Kernighan bit trick is in every optimisation textbook. Cache-aware programming is taught in systems courses. A skilled human programmer, given the same challenge and a few hours to research and implement, would arrive at substantially the same solution.



The AI advantage isn't \*intelligence\* — it's \*speed\*. Claude can produce a working segmented sieve in fifteen seconds. A human programmer might take an afternoon. But the final code quality, the algorithmic choices, the performance characteristics — these are comparable.



---



\## Humans Can Hold Their Own (For Now)



This experiment left me oddly optimistic about the future of human programmers.



Yes, AI coding assistants are impressive. Yes, they can produce working code faster than most humans. Yes, they have near-total recall of programming patterns and library APIs.



But they're not magic. They're not pulling novel algorithms from some vast space of unexplored possibilities. They're pattern-matching against their training data, applying techniques that humans developed and documented over decades.



The best AI-generated code in this experiment — Claude's compact, efficient sieve — was exactly what a skilled human would write, just produced faster. The worst AI-generated code — Gemini's and Grok's bug-laden iterations — was worse than what a competent junior developer would produce.



Meanwhile, the actual optimisations that broke the one-second barrier — parallel work-stealing, SIMD memory operations, cache-aligned segment sizing — came from human engineering knowledge applied on top of the AI outputs.



The current state of AI-assisted programming isn't "AI replaces programmers." It's "AI accelerates programmers." The vision, the architecture, the understanding of what makes code \*good\* rather than merely \*functional\* — these remain distinctly human contributions.



At least for now.



---



\## The Bottom Line



A 2,300-year-old algorithm, implemented with techniques from the 1970s through 2020s, running on silicon fabricated with processes developed over fifty years, can find fifty million prime numbers in under a quarter of a second.



No AI improved on that algorithm. The best AI systems implemented it well. The worst implemented it poorly.



What determines the difference between good and great code isn't raw intelligence or training data volume. It's \*care\* — the patience to handle edge cases, the discipline to use correct types, the wisdom to write code that future readers will understand.



Claude showed that care. ChatGPT showed taste. Gemini and Grok showed that capability on benchmarks doesn't always translate to capability in practice.



And Eratosthenes — who computed his sieve by hand, with stylus and wax tablet, in a library in Alexandria — showed that good algorithms are timeless.



The code that runs fastest on a billion-dollar cloud server in 2025 uses fundamentally the same logic that a Greek scholar invented before the Roman Empire existed.



Some things, it turns out, are worth getting right the first time.



---



\*All code from this experiment is available at \[github.com/whisprer/prime-shootout](https://github.com/whisprer/prime-shootout). The repository includes baseline implementations, all LLM-generated solutions, benchmark tooling, and the comprehensive techniques guide.\*



---



\*"In mathematics, simplicity is the ultimate sophistication. The sieve of Eratosthenes proves that a 2,300-year-old idea, properly implemented, can outperform 'clever' modern alternatives."\*



---


## Further Reading



\- "The Genuine Sieve of Eratosthenes" — Melissa E. O'Neill

\- "Segmented Sieve of Eratosthenes" — primesieve.org

\- "Prime Sieves using Binary Quadratic Forms" — Atkin \& Bernstein




\*\*About the Author\*\*



\*An algorithmic obsessive who believes that performance and elegance are not opposites. Founder of whispr.dev, where we think carefully about hard problems.\*



\*"May your sieves be cache-friendly and your bits be packed."\*

