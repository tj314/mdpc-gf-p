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

    auto set_coeff(fmpz_mod_poly_t p, unsigned idx, unsigned value, const CodeParams& params) -> void;
    auto set_coeff(fmpq_poly_t p, unsigned idx, unsigned value, const CodeParams& params) -> void;
    auto get_coeff(fmpz_mod_poly_t p, unsigned idx, const CodeParams& params) -> unsigned;
    auto get_coeff(fmpq_poly_t p, unsigned idx, const CodeParams& params) -> unsigned;

public:
    Random();
    ~Random();

    auto random_index(unsigned bound) -> unsigned;
    
    /*
    auto random_poly(fmpz_mod_poly_t output, const CodeParams& params) -> void;
    auto random_poly(fmpq_poly_t output, const CodeParams& params) -> void;
    */
    
    template<typename Poly>
    auto random_poly(Poly output,  const CodeParams& params, const unsigned add_to_first=0) -> void {
        unsigned third = params.k_value / 3;
        unsigned i;
        
        // set the first third of poly to 1
        // set the second third to -1
        // and the rest to 0
        for (i = 0; i < third; ++i) {
            set_coeff(output, i, 1, params);
        }
        for (; i < 2*third; ++i) {
            set_coeff(output, i, params.q_value-1, params);
        }
        for (; i < params.k_value; ++i) {
            set_coeff(output, i, 0, params);
        }
        
        unsigned tmp1, tmp2;
        
        // shuffle
        for (i = 0; i < params.k_value-1; ++i) {
            unsigned j = i + random_index(params.k_value - i);
            
            if (i == j) continue;
            else {
                tmp1 = get_coeff(output, i, params);
                tmp2 = get_coeff(output, j, params);
                set_coeff(output, i, tmp2, params);
                set_coeff(output, j, tmp1, params);
            }
        }
        if (add_to_first != 0) {
            tmp1 = get_coeff(output, 0, params) + add_to_first;
            if (tmp1 < 0) {
                tmp1 += params.q_value;
            } else if (tmp1 >= params.q_value) {
                tmp1 -= params.q_value;
            }
            set_coeff(output, 0, tmp1, params);
        }
        
        if (get_coeff(output, 0, params) == params.q_value) {
            std::cout << "wtf" << std::endl;
        }
    }
};
#endif