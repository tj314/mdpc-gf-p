#ifndef MCELIECE_QCMDPC_GF_P_RANDOM_HPP
#define MCELIECE_QCMDPC_GF_P_RANDOM_HPP

#define BACKEND_C

#ifdef BACKEND_C
#include <time.h>
#include <stdlib.h>

class Random {
public:
    Random();
    auto random_index(unsigned bound) -> unsigned;
};

#else
#include <flint.h>
#include <fmpz.h>

class Random {
private:
    flint_rand_t rnd;
    fmpz_t tmp;
    fmpz_t bound;

public:
    Random();
    ~Random();
    auto random_index(unsigned bound) -> unsigned;
};

#endif

#endif