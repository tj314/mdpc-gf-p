#ifndef MCELIECE_QCMDPC_GF_P_RANDOM_HPP
#define MCELIECE_QCMDPC_GF_P_RANDOM_HPP

#define BACKEND_C

#ifdef BACKEND_C
#include <time.h>
#include <stdlib.h>
#endif
#include <flint.h>
#include <fmpz.h>
#include <fmpq.h>
#include <fmpz_mod_poly.h>
#include <fmpq_poly.h>

#include <iostream>

#include "codeparams.hpp"

class Random {
private:
#ifndef BACKEND_C
    flint_rand_t rnd;
    fmpz_t bound;
#endif
    fmpz_t tmp;

public:
    Random();
    ~Random();

    auto random_index(unsigned bound) -> unsigned;
    
    auto random_poly(fmpz_mod_poly_t output, const CodeParams& params, unsigned add_to_first=0) -> void;
    auto random_poly(fmpq_poly_t output, const CodeParams& params, unsigned add_to_first=0) -> void;
};
#endif