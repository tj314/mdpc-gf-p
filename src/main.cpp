#include "code.hpp"
#include "random.hpp"

auto main() -> int {
    Random r;
    Code c{8, 11, r};
    return 0;
}
