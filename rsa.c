#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include "gmp.h"
#include "rsa.h"

void generate_prime(mpz_t rop, mpz_t max) {
    mpz_t rand, prime;
    gmp_randstate_t state;
    int class, i, randomData = open("/dev/urandom", O_RDONLY);
    char myRandomData[50];
    unsigned long seed = time(NULL);

    mpz_init(rand);
    mpz_init(prime);

    ssize_t result = read(randomData, myRandomData, (sizeof myRandomData) - 1);

    if (result < 0)
        exit(0);

    close(randomData);

    for (i = 0; i < 50; i++) {
        seed *= ((int) myRandomData[i]);
    }

    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);

    mpz_urandomm(rand, state, max);
    mpz_nextprime(prime, rand);

    //should probably have some check here to make sure that prime is actually prime
    //class = mpz_probab_prime_p(prime, 50);

    mpz_set(rop, prime);
}

void find_phi(mpz_t rop, mpz_t p, mpz_t q) {
    unsigned long one = 1;
    mpz_t pm, qm;

    mpz_init(pm);
    mpz_init(qm);

    mpz_sub_ui(pm, p, one);
    mpz_sub_ui(qm, q, one);

    mpz_mul(rop, pm, qm);
}

void generate_e(mpz_t rop, mpz_t n) {
    mpz_t e, gcd;
    unsigned long one = 1;

    mpz_init(e);
    mpz_init(gcd);
    
    do {
        generate_prime(e, n);

        mpz_gcd(gcd, e, n);

    } while (mpz_cmp_ui(gcd, one) != 0);

    mpz_set(rop, e);
}

void find_d(mpz_t rop, mpz_t a, mpz_t b) {
    mpz_t b0, t, q, x0, x1, temp, a2, b2;
    unsigned long zero = 0, one = 1;

    mpz_init(b0);
    mpz_init(t);
    mpz_init(q);
    mpz_init(x0);
    mpz_init(x1);
    mpz_init(temp);
    mpz_init(b2);
    mpz_init(a2);

    mpz_set(b0, b);
    mpz_set(a2, a);
    mpz_set(b2, b);
    mpz_set_ui(x0, zero);
    mpz_set_ui(x1, one);

    if (mpz_cmp_ui(b2, one) == 0) {
        mpz_set_ui(rop, one);
        return;
    }

    while (mpz_cmp_ui(a2, one) > 0) {
        mpz_tdiv_q(q, a2, b2);
        mpz_set(t, b2);
        mpz_mod(b2, a2, b2);
        mpz_set(a2, t);
        mpz_set(t, x0);
        mpz_mul(temp, q, x0);
        mpz_sub(x0, x1, temp);
        mpz_set(x1, t);
    }

    if (mpz_cmp_ui(x1, 0) < 0) {
        mpz_add(x1, x1, b0);
    }

    mpz_set(rop, x1);
}

void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    mpz_powm(c, m, e, n);
}

void decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    mpz_powm(m, c, d, n);
}

int main(int argc, char **argv) {
    mpz_t max, newmax, p, q, n, d, e, phi_n, c, m, m2;
    int class;

    mpz_init(max);
    mpz_init(newmax);
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(d);
    mpz_init(e);
    mpz_init(phi_n);
    mpz_init(c);
    mpz_init(m);
    mpz_init(m2);

    if (mpz_set_str(max, argv[1], 0) != 0) printf("Wrong input!\n");
    if (mpz_set_str(m, argv[2], 0) != 0) printf("Wrong input!\n");

    generate_prime(p, max);
    mpz_mul(newmax, max, p);
    generate_prime(q, newmax);

    mpz_mul(n, p, q);

    find_phi(phi_n, p, q); 
    generate_e(e, n);
    find_d(d, e, phi_n);

    encrypt(c, m, e, n);
    decrypt(m2, c, d, n);

    mpz_out_str(stdout, 10, p);
    printf("\n");
    mpz_out_str(stdout, 10, q);
    printf("\n");
    mpz_out_str(stdout, 10, phi_n);
    printf("\n");
    mpz_out_str(stdout, 10, e);
    printf("\n");
    mpz_out_str(stdout, 10, d);
    printf("\n");
    mpz_out_str(stdout, 10, c);
    printf("\n");
    mpz_out_str(stdout, 10, m2);

    return 0;
}
