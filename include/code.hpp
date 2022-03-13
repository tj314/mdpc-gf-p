#ifndef MCELIECE_QCMDPC_GF_P_CODE_HPP
#define MCELIECE_QCMDPC_GF_P_CODE_HPP

#include <flint.h>
#include <fmpq.h>
#include <fmpz.h>
#include <fmpz_mod_poly.h>
#include <fmpq_poly.h>
#include <iostream>
#include <cmath>
#include "random.hpp"

class Code {
private:
    CodeParams params;  // DO NOT MOVE THIS
                        // params MUST be the first member in this class
    fmpz_mod_poly_t h0, h1, h1_inv;

    // second block of G is supposed to be calculated as follows:
    // -(h1_inv*h0)^T
    // however, here, we omit the transposition
    // this is because it is unnecessary for the encoding to work
    fmpz_mod_poly_t second_block_G;

public:
    Code(unsigned q, unsigned k, Random& rnd);
    ~Code();
    auto init_keys(Random& rnd) -> void;
    auto encode() -> void;
    auto decode(void) -> void;
};

#endif
