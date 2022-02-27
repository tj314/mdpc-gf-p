#include "code.hpp"

Code::Code(unsigned q, unsigned k, Random& rnd) {
    // http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    if ((q & (q-1)) != 0 || q == 0) {
        // TODO: compile-time options for throwing exceptions vs printing errors
        std::cout << "Code constructor: Parameter q is not a power of 2!" << std::endl;
    }

    fmpz_init_set_ui(this->k, k);
    if (!fmpz_is_probabprime(this->k)) {
        // TODO: k is probably small enough to test deterministically
        std::cout << "Code constructor: Parameter k is probably not prime!" << std::endl;
    }
    
    fmpz_init_set_ui(this->q, q);
    fmpz_mod_ctx_init(ctx, this->q);
    fmpz_mod_poly_init(modulus, ctx);
    
    fmpz_mod_poly_set_coeff_ui(modulus, 0, q-1, ctx);
    fmpz_mod_poly_set_coeff_ui(modulus, k, 1, ctx);

    fmpz_mod_poly_print(modulus, ctx);
    flint_printf("\n");

    init_keys(rnd);
}

Code::~Code() {
    fmpz_mod_poly_clear(modulus, ctx);
    fmpz_mod_ctx_clear(ctx);
    fmpz_clear(q);
    fmpz_clear(k);
}

auto Code::generate_random_poly(fmpz_mod_poly_t output, Random& rnd) const -> void {
    unsigned k_val = fmpz_get_ui(k);
    unsigned third = k_val / 3;
    unsigned i;
    
    // set the first third of poly to 1
    // set the second third to -1
    // and the rest to 0
    for (i = 0; i < third; ++i) {
        fmpz_mod_poly_set_coeff_ui(output, i, 1, ctx);
    }
    for (; i < 2*third; ++i) {
        fmpz_mod_poly_set_coeff_ui(output, i, -1, ctx);
    }
    for (; i < k_val; ++i) {
        fmpz_mod_poly_set_coeff_ui(output, i, 0, ctx);
    }
    
    fmpz_t tmp;
    fmpz_init(tmp);
    unsigned tmp1, tmp2;
    
    // shuffle
    for (i = 0; i < k_val-1; ++i) {
        unsigned j = rnd.random_index(k_val);
        
        if (i == j) continue;
        else {
            fmpz_mod_poly_get_coeff_fmpz(tmp, output, i, ctx);
            tmp1 = fmpz_get_ui(tmp);
            fmpz_mod_poly_get_coeff_fmpz(tmp, output, j, ctx);
            tmp2 = fmpz_get_ui(tmp);
            fmpz_mod_poly_set_coeff_ui(output, i, tmp2, ctx);
            fmpz_mod_poly_set_coeff_ui(output, j, tmp1, ctx);
        }
    }
    fmpz_clear(tmp);
}

auto Code::init_keys(Random& rnd) -> void {
    fmpz_mod_poly_t h0, h1;
    fmpz_mod_poly_init(h0, ctx);
    fmpz_mod_poly_init(h1, ctx);
    generate_random_poly(h0, rnd);
    generate_random_poly(h1, rnd);
    fmpz_mod_poly_print(h0, ctx);
    flint_printf("\n");
    fmpz_mod_poly_print(h1, ctx);
    flint_printf("\n");
    fmpz_mod_poly_clear(h0, ctx);
    fmpz_mod_poly_clear(h1, ctx);
};

auto Code::encode() -> void {};

auto Code::decode(void) -> void {};