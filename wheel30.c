/* Variant B: wheel-30 sieve. Only numbers coprime to 30 are stored
 * (8 residues per 30: 1,7,11,13,17,19,23,29), so the array is ~27% the
 * size of the odd-only sieve and marking touches fewer bytes.
 * Index: for n, idx = (n/30)*8 + pos[n%30], pos[] = -1 for non-coprime.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const signed char POS[30] = {
    -1, 0,-1,-1,-1,-1,-1, 1,-1,-1,   /* 0..9   -> 1,7 */
    -1, 2,-1, 3,-1,-1,-1, 4,-1, 5,   /* 10..19 -> 11,13,17,19 */
    -1,-1,-1, 6,-1,-1,-1,-1,-1, 7,   /* 20..29 -> 23,29 */
};
static const unsigned char RES[8] = {1, 7, 11, 13, 17, 19, 23, 29};

int main(void) {
    char buf[32];
    ssize_t nread = read(STDIN_FILENO, buf, sizeof(buf) - 1);
    if (nread <= 0) return 1;
    buf[nread] = '\0';
    long N = 0;
    for (ssize_t i = 0; i < nread; i++) {
        char c = buf[i];
        if (c >= '0' && c <= '9') N = N * 10 + (c - '0');
        else if (N > 0) break;
    }
    if (N < 2) { printf("0\n"); return 0; }
    if (N < 7) { /* primes <= N among {2,3,5} */
        printf("%d\n", N >= 5 ? 3 : (N >= 3 ? 2 : 1));
        return 0;
    }

    long blocks = N / 30 + 1;
    long total = blocks * 8;
    unsigned char *is_comp = (unsigned char *)malloc((size_t)total);
    if (!is_comp) return 1;
    memset(is_comp, 0, (size_t)total);

    long sq = 1;
    while (sq * sq <= N) sq++;
    sq--;

    /* mark multiples of each prime p >= 7 (2,3,5 handled by wheel) */
    for (long p = 7; p <= sq; p++) {
        if (POS[p % 30] < 0) continue;
        long idx = (p / 30) * 8 + POS[p % 30];
        if (is_comp[idx]) continue;
        long p2 = p * p;
        /* mark p*q for q >= p, q coprime to 30: step through residues */
        for (long m = p2; m <= N; m += 2 * p) {
            /* only odd multiples; skip multiples of 3 and 5 via residue check */
            if (m % 3 == 0 || m % 5 == 0) continue;
            is_comp[(m / 30) * 8 + POS[m % 30]] = 1;
        }
    }

    long count = 3; /* primes 2, 3, 5 */
    for (long b = 0; b < blocks; b++) {
        for (int r = 0; r < 8; r++) {
            long v = b * 30 + RES[r];
            if (v > N) break;
            if (v < 7) continue;
            if (!is_comp[b * 8 + r]) count++;
        }
    }
    printf("%ld\n", count);
    return 0;
}
