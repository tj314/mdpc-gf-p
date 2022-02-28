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
/*
auto Random::random_poly(fmpz_mod_poly_t output,  const CodeParams& params) -> void {
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
}

auto Random::random_poly(fmpq_poly_t output,  const CodeParams& params) -> void {
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
        unsigned j = random_index(params.k_value);
        
        if (i == j) continue;
        else {
            fmpq_poly_get_coeff_fmpz(tmp, output, i);
            tmp1 = fmpz_get_ui(tmp);
            fmpq_poly_get_coeff_fmpz(tmp, output, j);
            tmp2 = fmpz_get_ui(tmp);
            set_coeff(output, i, tmp2, params);
            set_coeff(output, j, tmp1, params);
        }
    }
}
*/

auto Random::set_coeff(fmpz_mod_poly_t p, unsigned idx, unsigned value, const CodeParams& params) -> void {
    fmpz_mod_poly_set_coeff_ui(p, idx, value, params.ctx);
}

auto Random::set_coeff(fmpq_poly_t p, unsigned idx, unsigned value, const CodeParams& params) -> void {
    fmpq_poly_set_coeff_ui(p, idx, value);
}

auto Random::get_coeff(fmpz_mod_poly_t p, unsigned idx, const CodeParams& params) -> unsigned {
    fmpz_mod_poly_get_coeff_fmpz(tmp, p, idx, params.ctx);
    return fmpz_get_ui(tmp);
}

auto Random::get_coeff(fmpq_poly_t p, unsigned idx, const CodeParams& params) -> unsigned {
    fmpq_poly_get_coeff_fmpz(tmp, p, idx);
    return fmpz_get_ui(tmp);
}