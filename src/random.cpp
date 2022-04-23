#include "random.hpp"

Random::Random() {
    srand(time(nullptr));
}

auto Random::get() -> Random& {
    static Random instance;
    return instance;
}

auto Random::integer_internal(unsigned bound) -> unsigned {
    return rand() % bound;
}

auto Random::error_vector_internal(unsigned k) -> vector<int> {
    vector<int> error_vector;

    for (unsigned i = 0; i < 2*k; ++i) {
        error_vector.push_back(((int)this->integer_internal(3)) - 1);
    }

    return error_vector;
}

auto Random::integer(unsigned bound) -> unsigned {
    return get().integer_internal(bound);
}

auto Random::poly_internal(unsigned k, unsigned add_to_first) -> vector<int> {
    vector<int> poly;
    unsigned k3 = k / 3;
    for (unsigned i = 0; i < k; ++i) {
        if (i < k3) {
            poly.emplace_back(1);
        } else if (i >= k3 && i < 2*k3) {
            poly.emplace_back(-1);
        } else {
            poly.emplace_back(0);
        }
    }

    // shuffle
    for (unsigned i = 0; i < k; ++i) {
        unsigned j = this->integer_internal(k - i) + i;
        if (i != j) {
            std::iter_swap(poly.begin() + i, poly.begin() + j);
        }
    }

    if (add_to_first != 0) {
        poly.at(0) += (int)add_to_first;
    }
    return poly;
}

auto Random::poly(fmpq_polyxx& output, unsigned k, unsigned add_to_first) -> void {
    auto poly = get().poly_internal(k, add_to_first);
    for (unsigned i = 0; i < k; ++i) {
        output.set_coeff(i, ((slong)poly.at(i)));
    }
    /*
    for (unsigned i = 0; i < k; ++i) {
        std::cout << output.get_coeff(i) << " ";
    }
    std::cout << std::endl;
    */
}

auto Random::poly(fmpz_mod_polyxx& output, unsigned k, unsigned q, unsigned add_to_first) -> void {
    auto poly = get().poly_internal(k, add_to_first);
    for (unsigned i = 0; i < k; ++i) {
        long c = floor_mod(poly.at(i), q);
        output.set_coeff(i, c);
    }
}

auto Random::poly(vector<unsigned>& output, unsigned k, unsigned q, unsigned add_to_first) -> void {
    auto poly = get().poly_internal(k, add_to_first);
    if (!output.empty()) {
        output.clear();
    }
    for (int i: poly) {
        output.push_back(floor_mod(i, q));
    }
}

auto Random::error_vector(unsigned k) -> vector<int> {
    return get().error_vector_internal(k);
}

auto Random::reseed() -> void {
    srand(time(NULL));
}
