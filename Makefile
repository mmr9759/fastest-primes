CC      ?= gcc
CFLAGS  ?= -O3 -Wall -Wextra

all: primes

primes: main.c
	$(CC) $(CFLAGS) -o primes main.c

clean:
	rm -f primes primes_wheel

.PHONY: all clean
