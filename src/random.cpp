#include "random.hpp"

Random::Random() {
#ifdef BACKEND_C
    srand(time(NULL));
#else
    flint_randinit(rnd);
    fmpz_init(bound);
#endif
    fmpz_init(tmp);
}

Random::~Random() {
    fmpz_clear(tmp);
#ifndef BACKEND_C
    flint_randclear(rnd);
    fmpz_clear(bound);
#endif
}

auto Random::random_index(unsigned bound) -> unsigned {
#ifdef BACKEND_C
    return rand() % bound;
#else
    fmpz_set_ui(this->bound, bound);
    fmpz_randm(tmp, rnd, this->bound);
    return fmpz_get_ui(tmp);
#endif
}


auto Random::random_poly(fmpz_mod_poly_t output,  const CodeParams& params, unsigned add_to_first) -> void {
    unsigned third = params.k_value / 3;
    unsigned i;
    
    // set the first third of poly to 1
    // set the second third to -1
    // and the rest to 0
    for (i = 0; i < third; ++i) {
        fmpz_mod_poly_set_coeff_ui(output, i, 1, params.ctx);
    }
    for (; i < 2*third; ++i) {
        fmpz_mod_poly_set_coeff_ui(output, i, params.q_value-1, params.ctx);
    }
    for (; i < params.k_value; ++i) {
        fmpz_mod_poly_set_coeff_ui(output, i, 0, params.ctx);
    }
    
    unsigned tmp1, tmp2;
    
    // shuffle
    for (i = 0; i < params.k_value-1; ++i) {
        unsigned j = random_index(params.k_value);
        
        if (i == j) continue;
        else {
            fmpz_mod_poly_get_coeff_fmpz(tmp, output, i, params.ctx);
            tmp1 = fmpz_get_ui(tmp);
            fmpz_mod_poly_get_coeff_fmpz(tmp, output, j, params.ctx);
            tmp2 = fmpz_get_ui(tmp);
            fmpz_mod_poly_set_coeff_ui(output, i, tmp2, params.ctx);
            fmpz_mod_poly_set_coeff_ui(output, j, tmp1, params.ctx);
        }
    }
    
    if (add_to_first != 0) {
        fmpz_mod_poly_get_coeff_fmpz(tmp, output, 0, params.ctx);
        tmp1 = fmpz_get_ui(tmp);
        tmp1 += add_to_first;
        fmpz_mod_poly_set_coeff_ui(output, 0, tmp1, params.ctx);
    }
}

auto Random::random_poly(fmpq_poly_t output,  const CodeParams& params, unsigned add_to_first) -> void {
    unsigned third = params.k_value / 3;
    unsigned i;
    
    // set the first third of poly to 1
    // set the second third to -1
    // and the rest to 0
    for (i = 0; i < third; ++i) {
        fmpq_poly_set_coeff_si(output, i, 1);
    }
    for (; i < 2*third; ++i) {
        fmpq_poly_set_coeff_si(output, i, -1);
    }
    for (; i < params.k_value; ++i) {
        fmpq_poly_set_coeff_si(output, i, 0);
    }
    
    long tmp1, tmp2;
    
    // shuffle
    for (i = 0; i < params.k_value-1; ++i) {
        unsigned j = random_index(params.k_value - i) + i;
        
        if (i == j) continue;
        else {
            fmpq_poly_get_coeff_fmpz(tmp, output, i);
            tmp1 = fmpz_get_si(tmp);
            fmpq_poly_get_coeff_fmpz(tmp, output, j);
            tmp2 = fmpz_get_si(tmp);
            fmpq_poly_set_coeff_si(output, i, tmp2);
            fmpq_poly_set_coeff_si(output, j, tmp1);
        }
    }
    
    if (add_to_first != 0) {
        fmpq_poly_get_coeff_fmpz(tmp, output, 0);
        tmp1 = fmpz_get_si(tmp);
        tmp1 += add_to_first;
        fmpq_poly_set_coeff_si(output, 0, tmp1);
    }
}