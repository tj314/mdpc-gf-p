#include "protocol.hpp"

auto Protocol::encrypt(const vector<unsigned>& plaintext) -> optional<vector<unsigned>> {
    if (plaintext.size() != k) {
        return {};
    }
    /*
    vector<fmpzxx> vec{k};
    for (unsigned i = 0; i < k; ++i) {
        vec.push_back(fmpzxx{plaintext.at(i)});
    }
    vector<fmpzxx> encoded = c.encode(vec);
    vector<fmpzxx> error_vector = Random::error_vector();
    */
    // TODO: implement
    return {};
}

auto Protocol::decrypt(const vector<unsigned>& ciphertext) -> optional<vector<unsigned>> {
    // TODO: implement
    return {};
}
