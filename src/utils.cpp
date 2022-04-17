#include "utils.hpp"

auto floor_mod(long num, long modulo) -> long {
    num %= modulo;
    if (num < 0) {
        num += modulo;
    }
    return num;
}

