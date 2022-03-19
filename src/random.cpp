#include "random.hpp"
#include <cstdlib>
#include <ctime>

Random::Random() {
    srand(time(NULL));
}

auto Random::get() -> Random& {
    static Random instance;
    return instance;
}

auto Random::integer_internal(unsigned bound) -> unsigned {
    return rand() % bound;
}

auto Random::error_vector_internal(unsigned k) -> vector<fmpzxx> {
    vector<fmpzxx> error_vector;

    for (unsigned i = 0; i < 2*k; ++i) {
        error_vector.push_back(fmpzxx{this->integer_internal(2) - 1});
    }

    return error_vector;
}

auto Random::integer(unsigned bound) -> unsigned {
    return get().integer_internal(bound);
}

auto Random::poly(fmpq_polyxx& output, unsigned k, unsigned add_to_first) -> void {
    get().poly_internal<fmpq_polyxx, fmpqxx>(output, k, add_to_first);
}

auto Random::poly(fmpz_mod_polyxx& output, unsigned k, unsigned add_to_first) -> void {
    get().poly_internal<fmpz_mod_polyxx, fmpzxx>(output, k, add_to_first);
}

auto Random::error_vector(unsigned k) -> vector<fmpzxx> {
    return get().error_vector_internal(k);
}

auto Random::reseed() -> void {
    srand(time(NULL));
}
