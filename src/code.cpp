#include "code.hpp"

auto DecodeBounds::set(unsigned q) -> void {
        q_thirds = round(q / 3.0);
        q_2 = q / 2.0;
        q_6 = q / 6.0;
        q_18 = q / 18.0;
        b1 = ceil(q_6);
        b2 = floor(q_2);
        b3 = ceil(q_2);
        b4 = floor(5*q_6);
        b5 = ceil(q_18);
        b6 = floor(q_6);
        b7 = ceil(q_6);
        b8 = floor(5*q_18);
}

Code::Code(unsigned q, unsigned k):
    q_value(q),
    k_value(k),
    bounds(DecodeBounds()) {
    // http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    if ((q & (q-1)) != 0 || q == 0) {
        // TODO: compile-time options for throwing exceptions vs printing errors
        std::cout << "Code params: Parameter q is not a power of 2!" << std::endl;
    }
    fmpzxx kf{k};
    if (!kf.is_probabprime()) {
        // TODO: k is probably small enough to test deterministically
        std::cout << "Code params: Parameter k is probably not prime!" << std::endl;
    }
    init_keys();
}

auto Code::init_keys() -> void {
    bounds.set(q_value);
    unsigned h0_tick = round(q_value / 3.0);
    unsigned h1_tick = round(q_value / 9.0);

    fmpz_modxx_ctx context{q_value};
    fmpq_polyxx h1_tmp, modulo, f;
    fmpz_polyxx numerator;
    fmpz_mod_polyxx g{context}, h0_poly{context, k_value}, h1_poly{context, k_value}, h1_inv{context, k_value}, mod{context}, second_block_G_poly{context};
    fmpzxx m, r, q{q_value};

    if (!h0.empty()) {
        h0.clear();
    }

    // uniformly generate random h0 such that its length is k and its coefficients are from {-1, 0, 1}
    // then add h0_tick to the first coefficient
    Random::poly(h0, k_value, q_value, h0_tick);
    while (h0.size() < k_value) {
        h0.push_back(0);
    }
    for (unsigned i = 0; i < h0.size(); ++i) {
        h0_poly.set_coeff(i, floor_mod(h0.at(i), q_value));
    }

    // modulo is x^k - 1
    modulo.set_coeff(0, -1);
    modulo.set_coeff(k_value, 1);
    mod.set_coeff(0, q_value - 1);
    mod.set_coeff(k_value, 1);

    REGEN:
    // now uniformly generate random h1 such that its length is k and its coefficients are from {-1, 0, 1}
    // then add h1_tick to the first coefficient
    // additionally, h1 must be invertible
    while (true) {
        Random::poly(h1_tmp, k_value, h1_tick);

        // calculate poly f(x) such that
        // f = h_1^{-1} mod (x^k - 1) in Q[x]
        auto xgcd_result = xgcd(h1_tmp, modulo);
        if (!xgcd_result.get<0>().is_one()) {
            // gcd(h1_tmp, mod) != 1,
            // therefore f does not exist
            continue;
        }
        f = xgcd_result.get<1>();
        f %= modulo;
        f.canonicalise();

        // let m be denominator of f
        m = f.den();

        // calculate r such that
        // r = m^-1 mod q
        auto xgcd_result2 = xgcd(m, q);
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

    if (!h1.empty()) {
        h1.clear();
    }
    for (slong i = 0; i <= h1_tmp.degree(); ++i) {
        auto coeff = h1_tmp.get_coeff(i).num();
        int c = (int)coeff.to<slong>();
        h1.push_back((unsigned)floor_mod(c, q_value));
        h1_poly.set_coeff(i, coeff);
    }
    while (h1.size() < k_value) {
        h1.push_back(0);
    }

    fmpz_mod_polyxx pol{context, k_value};
    pol.set(h1_poly*h1_inv);
    pol %= mod;

    if (!pol.is_one()) {
        // std::cerr << "WTF?" << std::endl;
        goto REGEN;
    }

    second_block_G_poly.set(h1_inv * h0_poly);
    fmpzxx scalar{-1};
    second_block_G_poly = scalar * second_block_G_poly;
    second_block_G_poly = second_block_G_poly % mod;

    if (!second_block_G.empty()) {
        second_block_G.clear();
    }
    for (slong i = 0; i <= second_block_G_poly.degree(); ++i) {
        second_block_G.push_back(floor_mod(second_block_G_poly.get_coeff(i).to<slong>(), q_value));
    }
    while (second_block_G.size() < k_value) {
        second_block_G.push_back(0);
    }
}

auto Code::encode(const vector<long>& plaintext) -> vector<unsigned> {
    vector<unsigned> encoded;
    long tmp = 0;
    for (unsigned i = 0; i < k_value; ++i) {
        tmp = floor_mod(plaintext.at(i), q_value);
        encoded.push_back(tmp);
    }

    for (unsigned i = k_value; i > 0; --i) {
        tmp = 0;
        for (unsigned j = 0; j < k_value; ++j) {
            tmp += (plaintext.at(j) * second_block_G.at(floor_mod(i+j, k_value)));
        }
        tmp = floor_mod(tmp, q_value);
        encoded.push_back((unsigned)tmp);
    }
    return encoded;
}

auto Code::decode(const vector<long>& ciphertext, unsigned num_iterations) -> optional<vector<long>> {
    vector<unsigned> syndrome = calculate_syndrome(ciphertext);
    vector<long> error_vector(2*k_value, 0);

    bool syndrome_is_zero = true;
    for (unsigned i = 0; i < k_value; ++i) {
        syndrome_is_zero = syndrome_is_zero && (syndrome.at(i) == 0);
    }
    if (syndrome_is_zero) {
        return error_vector;
    }

    vector<unsigned> p(syndrome);
    vector<unsigned> esyn;
    long tmp;

    for (unsigned iter = 0; iter < num_iterations; ++iter) {
        decide(error_vector, p);
        transform(error_vector);
        esyn = calculate_syndrome(error_vector);

        // p = s - He^T = s - esyn
        syndrome_is_zero = true;
        for (unsigned i = 0; i < k_value; ++i) {
            tmp = (long)syndrome.at(i) - (long)esyn.at(i);
            tmp = floor_mod(tmp, q_value);
            p.at(i) = tmp;
            syndrome_is_zero = (p.at(i) == 0) && syndrome_is_zero;
        }
        
        if (syndrome_is_zero) {
            return error_vector;
        }
    }

    return {};
}

auto Code::calculate_syndrome(const vector<long>& ciphertext) -> vector<unsigned> {
    vector<unsigned> syndrome;
    long tmp;
    for (unsigned i = k_value; i > 0; --i) {
        tmp = 0;
        for (unsigned j = 0; j < k_value; ++j) {
            tmp += (h0.at(floor_mod(i + j, k_value)) * ciphertext.at(j));
        }
        tmp = floor_mod(tmp, q_value);
        for (unsigned j = 0; j < k_value; ++j) {
            tmp += (h1.at(floor_mod(i + j, k_value)) * ciphertext.at(k_value + j));
        }
        tmp = floor_mod(tmp, q_value);
        syndrome.push_back((unsigned)tmp);
    }
    return syndrome;
}

auto Code::decide(vector<long>& error_vector, const vector<unsigned>& syndrome) const -> void {
    for (unsigned i = 0; i < k_value; ++i) {
        unsigned p_i = syndrome.at(i);

        if (p_i >= bounds.b1 && p_i <= bounds.b2) {
            // e'_i = 1
            error_vector.at(i) += 1;
        } else if (p_i >= bounds.b3 && p_i <= bounds.b4) {
            // e'_i = -1
            error_vector.at(i) -= 1;
        }

        p_i %= bounds.q_thirds;
        if (p_i >= bounds.b5 && p_i <= bounds.b6) {
            // e'_ki = 1
            error_vector.at(k_value + i) += 1;
        } else if (p_i >= bounds.b7 && p_i <= bounds.b8) {
            // e'_ki = -1
            error_vector.at(k_value + i) -= 1;
        }
    }
}

auto Code::transform(vector<long>& error_vector) const -> void {
    for (unsigned i = 0; i < 2*k_value; ++i) {
        long& val = error_vector.at(i);
        if (val == 2) {
            val = -1;
        } else if (val == -2) {
            val = 1;
        }
    }
}

