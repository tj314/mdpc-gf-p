#include "code.hpp"
#include "codeparams.hpp"

// ==================
// CODE PARAMS CLASS
// ==================

auto CodeParams::set(unsigned q, unsigned k) -> void {
    k_value = k;
    q_value = q;
    
    // http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    if ((q & (q-1)) != 0 || q == 0) {
        // TODO: compile-time options for throwing exceptions vs printing errors
        std::cout << "Code params: Parameter q is not a power of 2!" << std::endl;
    }

    fmpz_init_set_ui(this->k, k);
    if (!fmpz_is_probabprime(this->k)) {
        // TODO: k is probably small enough to test deterministically
        std::cout << "Code params: Parameter k is probably not prime!" << std::endl;
    }
    
    fmpz_init_set_ui(this->q, q);
    fmpz_mod_ctx_init(ctx, this->q);
    fmpz_mod_poly_init(modulus, ctx);
    fmpz_mod_poly_set_coeff_ui(modulus, 0, q-1, ctx);
    fmpz_mod_poly_set_coeff_ui(modulus, k, 1, ctx);
}

CodeParams::~CodeParams() {
    fmpz_mod_poly_clear(modulus, ctx);
    fmpz_mod_ctx_clear(ctx);
    fmpz_clear(q);
    fmpz_clear(k);
}


// ==================
// CODE CLASS
// ==================

Code::Code(unsigned q, unsigned k, Random& rnd) {
    params.set(q, k);
    
    fmpz_mod_poly_init2(h0, k, params.ctx);
    fmpz_mod_poly_init2(h1, k, params.ctx);
    fmpz_mod_poly_init2(h1_inv, k, params.ctx);
    fmpq_poly_init2(h1_tmp, k);

    init_keys(rnd);
}

Code::~Code() {
    fmpz_mod_poly_clear(h0, params.ctx);
    fmpz_mod_poly_clear(h1, params.ctx);
    fmpz_mod_poly_clear(h1_inv, params.ctx);
    fmpq_poly_clear(h1_tmp);
}

auto Code::init_keys(Random& rnd) -> void {
    unsigned h0_prime = round(params.q_value / 3.0);
    unsigned h1_prime = round(params.q_value / 9.0);
    rnd.random_poly(h0, params, h0_prime); // coeffs from {-1, 0, 1} in Z_q

    fmpq_poly_t mod, gcd, S, T;
    fmpz_poly_t numerator;
    fmpz_mod_poly_t h1_inv_tmp;
    fmpz_t m, r, d, s;
    
    fmpz_poly_init(numerator);
    fmpz_mod_poly_init2(h1_inv_tmp, params.k_value, params.ctx);
    fmpq_poly_init2(mod, params.k_value);
    fmpq_poly_set_coeff_ui(mod, 0, params.q_value - 1);
    fmpq_poly_set_coeff_ui(mod, params.k_value, 1);
    fmpq_poly_init(gcd);
    fmpq_poly_init(S);
    fmpq_poly_init(T);
    fmpz_init(m);
    fmpz_init(r);
    fmpz_init(d);
    fmpz_init(s);

    char c;
    while (true) {
        rnd.random_poly(h1_tmp, params, h1_prime); // coeffs from {-1, 0, 1} in Q
        fmpq_poly_xgcd(gcd, S, T, h1_tmp, mod);
        // fmpq_poly_print_pretty(S, "x");

        if (!fmpq_poly_is_one(gcd)) {
            continue;
        }

        fmpq_poly_rem(T, S, mod);

        fmpq_poly_get_denominator(m, T);
        /*
        flint_printf("m = ");
        fmpz_print(m);
        flint_printf("\n");
        flint_printf("q = ");
        fmpz_print(params.q);
        flint_printf("\n");
        std::cin >> c;
        */
        fmpz_xgcd(d, r, s, m, params.q);
        if (!fmpz_is_one(d)) {
            flint_printf("d = ");
            fmpz_print(d);
            flint_printf("\n");
            continue;
        }
        
        std::cout << "loop" << std::endl;
        
        fmpq_poly_get_numerator(numerator, T);
        fmpz_mod_poly_set_fmpz_poly(h1_inv_tmp, numerator, params.ctx);
        fmpz_mod_poly_scalar_mul_fmpz(h1_inv, h1_inv_tmp, r, params.ctx);
        break;
    }
    
    fmpq_poly_clear(mod);
    fmpq_poly_clear(gcd);
    fmpq_poly_clear(S);
    fmpq_poly_clear(T);
    fmpz_clear(m);
    fmpz_clear(r);
    fmpz_clear(d);
    fmpz_clear(s);
    fmpz_poly_clear(numerator);
    fmpz_mod_poly_clear(h1_inv_tmp, params.ctx);
    
    fmpz_mod_poly_print_pretty(h0, "x", params.ctx);
    flint_printf("\n");
    fmpq_poly_print_pretty(h1_tmp, "x");
    flint_printf("\n");
    fmpz_mod_poly_print_pretty(h1_inv, "x", params.ctx);
    flint_printf("\n");
};

auto Code::encode() -> void {};

auto Code::decode(void) -> void {};