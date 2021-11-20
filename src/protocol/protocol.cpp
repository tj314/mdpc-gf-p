#include "protocol.hpp"

Protocol::Protocol(Code code): code(std::move(code)) {}

auto Protocol::generate_error_vector() -> vec_ZZ_p {
    long err_len = code.get_k() * 2;
    vec_ZZ_p err;
    err.FixLength(err_len);
    for (long i = 0; i < err_len; ++i) {
        err[i] = ZZ_p(RandomBnd(3L) - 1);
    }
    return err;
}

auto Protocol::encrypt(const vec_ZZ_p& open_text) -> vec_ZZ_p {
    vec_ZZ_p encoded = code.encode(open_text);
    vec_ZZ_p err = generate_error_vector();
    return encoded + err;
}

auto Protocol::decrypt(const vec_ZZ_p& cipher_text) -> vec_ZZ_p {
    vec_ZZ_p err = code.decode(cipher_text, 5);
    return VectorCopy(cipher_text - err, code.get_k());
}