CC = gcc
CFLAGS = -std=gnu99 -lgmp -Wall

all: kmov rsa

kmov: kmov_generalized_gmp.o
	$(CC) $(CFLAGS) kmov_generalized_gmp.c -o kmov
rsa: rsa_gmp.o
	$(CC) $(CFLAGS) rsa_gmp.c -o rsa


.PHONY: clean
clean:
	rm -rf *o kmov rsa
