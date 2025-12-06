5e5
10-run mean of per-run Avg (ms):

| Program                        | Mean Avg (ms)| Std of Avgs | Runs |
|--------------------|-------:|----------:|----:|
| c-primes-claude             | 24.900      | 4.049                 | 10       |
| c-primes-gpetey            | 25.203       | 6.645                 | 10       |
| c-primes-grok                  | 25.569       | 3.985                 | 10        |
| c-primes-gemini             | 29.656      | 10.418              | 10        |
| r-primes-fast                  | 31.447       | 5.914                 | 10        |
| r-primes                                  | 37.759      | 19.462              | 10        |
| c-primes-fast                  | 37.956       | 23.508              | 10        |
| c-primes                                  | 40.550      | 17.102              | 10        |
| p-primes-smart-50k   | 2502.156 | 303.847           | 10        |
| p-primes-smart-500   | 2524.253 | 287.170           | 10        |
| p-primes-smart-500k | 3125.962 | 325.401          | 10       |

$


1e9
Starting benchmarks with 10 runs, 10 repeats per run...
Progress: Run 10/10 - Testing: c-primes-fast-claude-seg-1e9

Program                  							Avg (ms)       	Min (ms)       	Max (ms)      	 StdDev (ms)
----------------------------------------------------------------------
c-primes-1e9             					16737.742      15433.224     	17457.998      525.273
c-primes-fast-1e9        				3719.541       	3612.038      	4013.302       	115.376
r-primes-1e9             					19458.010      14370.344      	20654.367      1716.092
r-primes-fast-1e9        				2265.822       	2156.453       	2579.732       	115.027
c-primes-fast-gpetey-1e9 		3757.166       	3633.551       	3932.952       	90.778
c-primes-fast-claude-1e9 		3170.096       	3085.333       	3300.263       	52.586
c-primes-fast-gpetey-seg-1e9	928.519        	897.319        	983.531        	22.153
c-primes-fast-claude-seg-1e9	1143.791       	1129.323       	1167.189       	11.762

phine@wofl-BFT0 MINGW64 /d/code/primes-compare/primes\_ben/dist

$


aux 1e9
10-run mean of per-run Avg (ms):

| Program                                              | Mean Avg (ms) | Std of Avgs | Runs |
|-----------------------|------------:|---------:|-----:|
| c-primes-the-beast-1e9 | 12.389                     | 0.298               | 10        |
| c-primes-bitpacked-1e9 | 12.596                    | 0.715                | 10        |
| c-primes-parallel-1e9    | 12.654                     | 0.873               | 10        |
| r-primes-wheel-1e9             | 12.989                     | 1.013               | 10        |

phine@wofl-BFT0 MINGW64 /d/code/prime-shootout/build/dist/round-Aux
$


simd + parallel 1e9
10-run mean of per-run Avg (ms):

| Program                                           | Mean Avg (ms)          | Std of Avgs | Runs  |
|----------------------|---------------:|----------:|----:|
| c-primes-parallel-1e9 | 0.719.434                       | 16.270             | 10       |
| c-primes-simd-1e9             | 1,448.903                       | 17.124             | 10       |

$

