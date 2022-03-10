#ifndef MCELIECE_QCMDPC_GF_P_RANDOM_HPP
#define MCELIECE_QCMDPC_GF_P_RANDOM_HPP

#include <time.h>
#include <stdlib.h>
#include <flintxx.h>
#include <fmpzxx.h>
#include <fmpqxx.h>
#include <fmpz_mod_polyxx.h>
#include <fmpq_polyxx.h>
#include <iostream>
#include "codeparams.hpp"

using flint::fmpq_polyxx;
using flint::fmpzxx;
using flint::fmpz_mod_polyxx;

class Random {
public:
    Random();

    auto random_index(unsigned bound) -> unsigned;

    template<typename Poly>
    auto random_poly(Poly output, const CodeParams& params, unsigned add_to_first = 0) -> void {
        unsigned third = params.k_value / 3;
        unsigned i;

        // set the first third of poly to 1
        // set the second third to -1
        // and the rest to 0
        for (i = 0; i < third; ++i) {
            output.set_coeff(i, 1);
        }
        for (; i < 2*third; ++i) {
            output.set_coeff(i, -1);
        }
        for (; i < params.k_value; ++i) {
            output.set_coeff(i, 0);
        }

        fmpzxx tmp1, tmp2;

        // shuffle
        for (i = 0; i < params.k_value-1; ++i) {
            unsigned j = random_index(params.k_value - i) + i;

            if (i == j) {
                continue;
            } else {
                tmp1 = output.get_coeff(i);
                tmp2 = output.get_coeff(j);
                output.set_coeff(i, tmp2);
                output.set_coeff(j, tmp1);
            }
        }

        if (add_to_first != 0) {
            tmp1 = output.get_coeff(0);
            tmp1 += add_to_first;
            output.set_coeff(0, tmp1);
        }
    }
};
#endif
