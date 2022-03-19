#ifndef MCELIECE_QCMDPC_GF_P_PROTOCOL_HPP
#define MCELIECE_QCMDPC_GF_P_PROTOCOL_HPP

#include <fmpzxx.h>
#include <vector>
#include <optional>
#include <iostream>
#include "code.hpp"

using flint::fmpzxx;
using std::vector;
using std::optional;
using std::cerr;
using std::endl;

class Protocol {
private:
    unsigned q, k;
    Code c;

public:
    Protocol(unsigned q, unsigned k): q(q), k(k), c(Code{q, k}) {}
    auto encrypt(const vector<unsigned>& plaintext, bool verbose=false) -> optional<vector<unsigned>>;
    auto decrypt(const vector<unsigned>& ciphertext, unsigned num_iterations=10, bool verbose=false) -> optional<vector<unsigned>>;
};
#endif
