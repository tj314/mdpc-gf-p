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
    h0.reserve(k_value);
    h1.reserve(k_value);
    init_keys();
}

auto Code::init_keys() -> void {
    bounds.set(q_value);
    unsigned h0_tick = round(q_value / 3.0);
    unsigned h1_tick = round(q_value / 9.0);

    h0.clear();
    h1.clear();
    for (unsigned i = 0; i < k_value; ++i) {
        h0.push_back(floor_mod(((int)Random::integer(3)) - 1, q_value));
        h1.push_back(floor_mod(((int)Random::integer(3)) - 1, q_value));
    }

    h0.at(0) += h0_tick;
    h1.at(0) += h1_tick;

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

