#include "random.hpp"

Random::Random() {
    srand(time(NULL));
}

auto Random::random_index(unsigned bound) -> unsigned {
    return rand() % bound;
}
