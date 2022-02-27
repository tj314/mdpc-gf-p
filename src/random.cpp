#include "random.hpp"

#ifdef BACKEND_C
Random::Random() {
    srand(time(NULL));
}

auto Random::random_index(unsigned bound) -> unsigned {
    return rand() % bound;
}

#else
Random::Random() {
    flint_randinit(rnd);
    fmpz_init(tmp);
    fmpz_init(bound);
}

Random::~Random() {
    flint_randclear(rnd);
    fmpz_clear(tmp);
    fmpz_clear(bound);
}

auto Random::random_index(unsigned bound) -> unsigned {
    fmpz_set_ui(this->bound, bound);
    fmpz_randm(tmp, rnd, this->bound);
    return fmpz_get_ui(tmp);
}
#endif