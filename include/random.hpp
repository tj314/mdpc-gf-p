#ifndef MCELIECE_QCMDPC_GF_P_RANDOM_HPP
#define MCELIECE_QCMDPC_GF_P_RANDOM_HPP

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