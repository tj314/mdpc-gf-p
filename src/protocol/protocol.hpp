#ifndef MCELIECE_QCMDPC_GF_P_PROTOCOL_H
#define MCELIECE_QCMDPC_GF_P_PROTOCOL_H

#include <NTL/ZZ_p.h>
#include <NTL/vec_ZZ_p.h>

#include "code.hpp"

NTL_CLIENT

/**
 * This class provides encryption and decryption using a given code.
 */
class Protocol {
private:
	Code code;
public:

	/**
	 * @brief Constructs a Protocol instance using the given code.
	 *
	 * Parameter code will be moved!
	 *
	 * @param code instance of Code
	 */
	Protocol(Code code);

	/**
	 * @brief Generate random vector from I1 of length 2*k, where k is a parameter of used code.
	 *
	 * https://libntl.org/doc/ZZ.cpp.html section Pseudo-Random Numbers
	 *
	 * @return generated vector
	 */
	auto generate_error_vector() -> vec_ZZ_p;

	/**
	 * @brief Encrypts given message.
	 *
	 * encrypted = code.encode(message) + error_vector
	 *
	 * @param open_text vector containing the message
	 * @return vector containing encrypted message
	 */
	auto encrypt(const vec_ZZ_p& open_text) -> vec_ZZ_p;

	// TODO: Implement
	auto decrypt(const vec_ZZ_p& cipher_text) -> vec_ZZ_p;
		//1. decode message using code.decode(message, code.getParityCheckMatrix(), MAX_ITER)
		//2. read first n bits form decoded message
		//3. return plaintext
		// decoding may fail ====> decryption may fail ====> throw an exception
};

#endif //MCELIECE_QCMDPC_GF_P_PROTOCOL_H
