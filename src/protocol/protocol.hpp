#ifndef MCELIECE_QCMDPC_GF_P_PROTOCOL_H
#define MCELIECE_QCMDPC_GF_P_PROTOCOL_H

#include <NTL/ZZ_p.h>
#include <NTL/vec_ZZ_p.h>

NTL_CLIENT

class Protocol {
private:
    Code code;
public:
    Protocol(code, message) {
        this.code = code;
        this.message = message;
    }

    // generate random sequence from Id={-1, 0, 1} of length (?)
    // https://libntl.org/doc/ZZ.cpp.html section Pseodo-Random Numbers
    Vec<ZZ> generate_error_vector() {
        //
    }

    // encrypts message, returns ciphertext
    Vec<ZZ> encrypt() {
        //1. generate e - error vector of length k from Id={-1, 0, 1}
        //2. encode message m by multiplying it with generator matrix
        //3. return ciphertext containing encoded_message + e
    }

    // decrypts message, returns plaintext
    Vec<ZZ> decrypt() {
        //1. decode message using code.decode(message, code.getParityCheckMatrix(), MAX_ITER)
        //2. read first n bits form decoded message
        //3. return plaintext
    }
}

#endif //MCELIECE_QCMDPC_GF_P_PROTOCOL_H
