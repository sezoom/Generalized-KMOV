CC = gcc
CFLAGS = -std=gnu99 -lgmp -Wall

all: kmov

kmov: kmov_generalized_gmp.o
	$(CC) $(CFLAGS) kmov_generalized_gmp.c -o kmov

.PHONY: clean
clean:
	rm -rf *o kmov
