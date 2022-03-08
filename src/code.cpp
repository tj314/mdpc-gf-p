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

    fmpq_poly_t mod, gcd, f, T;
    fmpz_poly_t numerator;
    fmpz_mod_poly_t g;
    fmpz_t m, r, d, s;
    
    fmpz_poly_init(numerator);
    fmpz_mod_poly_init2(g, params.k_value, params.ctx);
    fmpq_poly_init2(mod, params.k_value);
    fmpq_poly_set_coeff_si(mod, 0, - 1);
    fmpq_poly_set_coeff_si(mod, params.k_value, 1);
    fmpq_poly_init(gcd);
    fmpq_poly_init(f);
    fmpq_poly_init(T);
    fmpz_init(m);
    fmpz_init(r);
    fmpz_init(d);
    fmpz_init(s);

    while (true) {
        // coeffs from {-1, 0, 1} in Q + h1_prime added to the first coeff
        rnd.random_poly(h1_tmp, params, h1_prime);

        // calculate poly f(x) such that
        // f = h_1^{-1} mod (x^k - 1) in Q[x]
        fmpq_poly_xgcd(gcd, f, T, h1_tmp, mod);
        if (!fmpq_poly_is_one(gcd)) {
            // f does not exist
            continue;
        }
        fmpq_poly_rem(f, f, mod);

        // let m be denominator of f
        fmpq_poly_get_denominator(m, f);

        // calculate r such that
        // r = m^-1 mod q
        fmpz_xgcd(d, r, s, m, params.q);
        if (!fmpz_is_one(d)) {
            // r does not exist
            continue;
        }
        
        // calculate poly g(x) such that
        // g(x) = m*f(x) mod q
        fmpq_poly_get_numerator(numerator, f);
        fmpz_mod_poly_set_fmpz_poly(g, numerator, params.ctx);
        // fmpz_mod_poly_scalar_mul_fmpz(g, g, m, params.ctx);
        
        // calculate poly h(x) such that
        // h(x) = r*g(x) mod q
        // then h(x) is h^{-1}(x) in Z_q[x]/(x^k - 1)
        fmpz_mod_poly_scalar_mul_fmpz(h1_inv, g, r, params.ctx);
        break;
    }
    
    fmpq_poly_get_numerator(numerator, h1_tmp);
    fmpz_mod_poly_set_fmpz_poly(h1, numerator, params.ctx);
    
    fmpq_poly_clear(mod);
    fmpq_poly_clear(gcd);
    fmpq_poly_clear(f);
    fmpq_poly_clear(T);
    fmpz_clear(m);
    fmpz_clear(r);
    fmpz_clear(d);
    fmpz_clear(s);
    fmpz_poly_clear(numerator);
    fmpz_mod_poly_clear(g, params.ctx);

    /*
    flint_printf("h0: ");
    fmpz_mod_poly_print_pretty(h0, "x", params.ctx);
    flint_printf("\n");

    flint_printf("h1: ");
    fmpz_mod_poly_print_pretty(h1, "x", params.ctx);
    flint_printf("\n");

    flint_printf("h1^-1: ");
    fmpz_mod_poly_print_pretty(h1_inv, "x", params.ctx);
    flint_printf("\n");

    fmpz_mod_poly_t tmp;
    fmpz_mod_poly_init(tmp, params.ctx);
    fmpz_mod_poly_mul(tmp, h1, h1_inv, params.ctx);
    flint_printf("h1 * h1^-1 = ");
    fmpz_mod_poly_print_pretty(tmp, "x", params.ctx);
    flint_printf("\n");
    fmpz_mod_poly_clear(tmp, params.ctx);
    */
};

auto Code::encode() -> void {};

auto Code::decode(void) -> void {};
