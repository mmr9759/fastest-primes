/* Fastest correct prime counter: pi(N) for 1 <= N <= 5,000,000.
 *
 * Odd-only Sieve of Eratosthenes on a byte bitset.
 * - is_composite[i] covers odd number 2*i+1 (i = 0..(N-1)/2).
 * - memset to 0, mark multiples of each odd prime p starting at p*p,
 *   stepping 2p (only odd multiples).
 * - Counting pass over the bitset.
 *
 * Fast I/O: raw read() + manual parse, printf for the single number.
 * No CPU-specific flags required: plain -O3 builds anywhere.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    if (N < 2) {
        printf("0\n");
        return 0;
    }

    /* number of odd integers in [1, N] */
    long half = (N + 1) / 2;
    unsigned char *is_comp = (unsigned char *)malloc((size_t)half);
    if (!is_comp) return 1;
    memset(is_comp, 0, (size_t)half);

    long sq = 1;
    while (sq * sq <= N) sq++;
    sq--; /* floor(sqrt(N)) */

    /* sieve: p odd, index i = (p-1)/2; p*p index = (p*p-1)/2; step = p */
    for (long p = 3; p <= sq; p += 2) {
        if (!is_comp[(p - 1) / 2]) {
            long start = (p * p - 1) / 2;
            for (long j = start; j < half; j += p)
                is_comp[j] = 1;
        }
    }

    long count = 1; /* prime 2 */
    for (long i = 1; i < half; i++)
        if (!is_comp[i]) count++;

    printf("%ld\n", count);
    return 0;
}
