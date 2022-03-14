#ifndef MCELIECE_QCMDPC_GF_P_PROTOCOL_HPP
#define MCELIECE_QCMDPC_GF_P_PROTOCOL_HPP

#include <fmpzxx.h>
#include <vector>
#include <optional>
#include "code.hpp"

using flint::fmpzxx;
using std::vector;
using std::optional;

class Protocol {
private:
    unsigned q, k;
    Code c;

public:
    Protocol(unsigned q, unsigned k): q(q), k(q), c(Code{q, k}) {}
    auto encrypt(const vector<unsigned>& plaintext) -> optional<vector<unsigned>>;
    auto decrypt(const vector<unsigned>& ciphertext) -> optional<vector<unsigned>>;
};
#endif
