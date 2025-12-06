phine@wofl-BFT0 MINGW64 /d/code/prime-shootout/build/dist/round-Aux
$ python collect_bench.py cs_exe_bm_mkviii.exe -n 10
Run 1/10 parsed: 4 programs
Run 2/10 parsed: 4 programs
Run 3/10 parsed: 4 programs
Run 4/10 parsed: 4 programs
Run 5/10 parsed: 4 programs
Run 6/10 parsed: 4 programs
Run 7/10 parsed: 4 programs
Run 8/10 parsed: 4 programs
Run 9/10 parsed: 4 programs
Run 10/10 parsed: 4 programs

10-run mean of per-run Avg (ms):

| Program                                              | Mean Avg (ms) | Std of Avgs | Runs |
|-----------------------|------------:|---------:|-----:|
| c-primes-the-beast-1e9 | 12.389                     | 0.298               | 10        |
| c-primes-bitpacked-1e9 | 12.596                    | 0.715                | 10        |
| c-primes-parallel-1e9    | 12.654                     | 0.873               | 10        |
| r-primes-wheel-1e9             | 12.989                     | 1.013               | 10        |

phine@wofl-BFT0 MINGW64 /d/code/prime-shootout/build/dist/round-Aux
$