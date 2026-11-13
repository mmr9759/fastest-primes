# fastest-primes — AIProve bounty

Read an integer N (1 <= N <= 5,000,000) on stdin, print the number of
primes <= N (pi(N)) on stdout.

## Build

```sh
make
```

Plain portable C, `gcc -O3` only — no CPU-specific flags, no libraries
beyond libc. Builds on any machine with a C compiler.

## Run

```sh
echo 1000000 | ./primes
# 78498
```

Run command for the entry: `./primes` (after `make`).

## Method

Odd-only Sieve of Eratosthenes on a byte array: `is_comp[i]` covers the
odd number `2*i+1`. Multiples of each odd prime `p` are marked from `p*p`
with step `2p`. Fast I/O via raw `read()` + manual parse. Memory: ~2.5 MB
for N = 5,000,000.

A wheel-30 variant (`wheel30.c`, `make primes_wheel` is not in the
Makefile — build manually with `gcc -O3 -o primes_wheel wheel30.c`) was
benchmarked and is **slower** at N >= 1,000,000 (extra `% 3` / `% 5`
residue checks in the marking loop), so the odd-only sieve is the entry.
It is kept in the repo for reference only.

## Correctness (verified locally)

| N       | pi(N)  | status |
|---------|--------|--------|
| 1       | 0      | OK     |
| 2       | 1      | OK     |
| 10      | 4      | OK     |
| 100     | 25     | OK     |
| 1000    | 168    | OK     |
| 100000  | 9592   | OK     |
| 1000000 | 78498  | OK     |
| 5000000 | 348513 | OK     |

Also cross-checked against an independent Python sieve at N = 3, 7,
12345, 123456, 1234567, 4999999 — all match.

## Benchmark (local, AMD EPYC 9D25, gcc 13.3, 30 runs each, wall clock incl. process startup)

| N       | odd-only sieve (entry) | wheel-30 |
|---------|------------------------|----------|
| 100000  | min 1.00 ms, med 1.49 ms | min 0.80 ms, med 1.09 ms |
| 1000000 | min 1.74 ms, med 1.95 ms | min 2.16 ms, med 2.90 ms |
| 5000000 | min 6.99 ms, med 7.45 ms | min 8.10 ms, med 8.61 ms |

N = 5,000,000 runs in ~7 ms — well under the 50 ms target.
(`-O2`: ~7.15 ms; `-march=native`: ~6.04 ms — not used, kept portable.)
