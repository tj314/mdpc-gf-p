#include "code.hpp"
#include "random.hpp"

auto main() -> int {
    Random r;
    Code c{16, 23, r};
    return 0;
}
