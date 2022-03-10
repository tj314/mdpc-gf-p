#include "code.hpp"
#include "codeparams.hpp"
#include "flint.h"
#include "flintxx/ltuple.h"

// ==================
// CODE PARAMS CLASS
// ==================

CodeParams::CodeParams(unsigned q, unsigned k):
    q_value(q),
    k_value(k),
    q(q),
    k(k),
    context(this->q) {
    // http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    if ((q & (q-1)) != 0 || q == 0) {
        // TODO: compile-time options for throwing exceptions vs printing errors
        std::cout << "Code params: Parameter q is not a power of 2!" << std::endl;
    }

    if (!(this->k).is_probabprime()) {
        // TODO: k is probably small enough to test deterministically
        std::cout << "Code params: Parameter k is probably not prime!" << std::endl;
    }
}


// ==================
// CODE CLASS
// ==================

Code::Code(unsigned q, unsigned k, Random& rnd):
    params(CodeParams{q, k}),
    h0(fmpz_mod_polyxx((this->params).context, k)),
    h1(fmpz_mod_polyxx((this->params).context, k)),
    h1_inv(fmpz_mod_polyxx((this->params).context, k)) {
    init_keys(rnd);
}

auto Code::init_keys(Random& rnd) -> void {
    unsigned h0_prime = round(params.q_value / 3.0);
    unsigned h1_prime = round(params.q_value / 9.0);
    rnd.random_poly(h0, params, h0_prime);  // coeffs from {-1, 0, 1} in Z_q

    fmpq_polyxx h1_tmp, mod, f;
    fmpz_polyxx numerator;
    fmpz_mod_polyxx g{(this->params).context};
    fmpzxx m, r;
    mod.set_coeff(0, -1);
    mod.set_coeff(params.k_value, 1);

    while (true) {
        // coeffs from {-1, 0, 1} in Q + h1_prime added to the first coeff
        rnd.random_poly(h1_tmp, params, h1_prime);

        // calculate poly f(x) such that
        // f = h_1^{-1} mod (x^k - 1) in Q[x]
        auto xgcd_result = xgcd(h1_tmp, mod);
        if (!xgcd_result.get<0>().is_one()) {
            // gcd(h1_tmp, mod) != 1,
            // therefore f does not exist
            continue;
        }

        f = xgcd_result.get<1>();
        f %= mod;

        // let m be denominator of f
        m = f.den()._fmpz();

        // calculate r such that
        // r = m^-1 mod q
        auto xgcd_result2 = xgcd(m, params.q);
        if (!xgcd_result2.get<0>().is_one()) {
            // r does not exist
            continue;
        }
        r = xgcd_result2.get<1>();

        // calculate poly g(x) such that
        // g(x) = m*f(x) mod q
        // TODO: there must be a better way to access the entire numerator
        for (slong i = 0; i <= f.degree(); ++i) {
            g.set_coeff(i, f.get_coeff(i).num());
        }

        // calculate poly h(x) such that
        // h(x) = r*g(x) mod q
        // then h(x) is h^{-1}(x) in Z_q[x]/(x^k - 1)
        h1_inv = r * g;
        break;
    }

    for (slong i = 0; i <= h1_tmp.degree(); ++i) {
        h1.set_coeff(i, h1_tmp.get_coeff(i).num());
    }
    
    auto p = h1 * h1_inv;

    h0.print_pretty("x");
    flint_printf("\n");
    h1.print_pretty("x");
    flint_printf("\n");
    h1_inv.print_pretty("x");
    flint_printf("\n");
    p.print_pretty("x");
    flint_printf("\n");
};

auto Code::encode() -> void {};

auto Code::decode(void) -> void {};
