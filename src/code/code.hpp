#ifndef MCELIECE_QCMDPC_GF_P_CODE_HPP
#define MCELIECE_QCMDPC_GF_P_CODE_HPP

#include <vector>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/tools.h>
#include <NTL/vec_ZZ_p.h>
#include <NTL/ZZ_pEX.h>

NTL_CLIENT
using namespace std;

class Code {
private:
	long q; // power of 2
	long k; // length of cyclic block (H)

	// matrices represented by polynomials
	// type (?) ZZ_pEX: univariate polynomials over ZZ_pE(ring/field extension over ZZ_p)
	ZZ_pEX h0, h0_inverse, h1;
	ZZ_pEX g;

	/**
	 * @brief Generate a polynomial mod (x^k - 1) with coefficients drawn from I1 and sum equal to 0.
	 * 
	 * @return vec_ZZ_p 
	 */
    auto generate_random_poly() -> ZZ_pEX;

	auto generate_h0() -> void;

	auto generate_h1() -> void;

	/**
	// generate cyclic block from hi
	ZZ_pEX generate_cyclic_block();
	*/

	auto generate_private_key() -> void;

	// creates public key G from identity matrix, inverted H0^-1, H1
	auto generate_public_key() -> void;

public:
	Code(unsigned q, unsigned k);

	// CODING FUNCTIONS
	// encodes message by multiplying with generator matrix G 
	// returns message * G
	auto encode(vec_ZZ_p message) -> vec_ZZ_p;

	// decodes message using p-ary bit-flipping
	// returns e (error vector) or decode error
	auto decode(vec_ZZ_p message, const size_t max_iter) -> vec_ZZ_p;
		// uses ciphertext message, parity check matrix H, MAX_ITER
		// 1. implement decoding algorithm
		// 2. return e or decode error 
};


#endif