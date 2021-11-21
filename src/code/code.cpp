#include "code.hpp"

/*-----------*\
 CONSTRUCTORS
\*-----------*/

Code::Code(unsigned q, unsigned k) : q((long)q), k((long)k) {
	// http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
	if ((q & (q-1)) != 0 || q == 0) {
		// TODO compile-time options for throwing exceptions vs printing errors
		cout << "Code constructor: Parameter q is not a power of 2!" << endl;
	}
	// ProbPrime returns 1 if k is prime
	if (!ProbPrime((long)k, 3)) { // TODO: k is probably small enough to test deterministically
		// TODO same as above
		cout << "Code constructor: Parameter k is probably not a prime!" << endl;
	}

	ZZ_p::init(ZZ(q));

	SetCoeff(modulus, 0, -1);
	SetCoeff(modulus, k);

	init_keys();
}


/*-----------*\
 PRIVATE
\*-----------*/


auto Code::generate_random_poly() const -> ZZ_pX {
	vector<long> vec;
	long sum = 0;
	for (size_t i = 0; i < (size_t)k; ++i) {
		long tmp = RandomBnd(3L) - 1;
		sum += tmp;
		vec.push_back(tmp);
	}
	
	while (sum != 0) {
		long idx = RandomBnd(k);
		long tmp = RandomBnd(3L) - 1;
		sum -= vec[idx];
		sum += tmp;
		vec[idx] = tmp;
	}

	ZZ_pX out;
	for (size_t i = 0; i < (size_t)k; ++i)
		SetCoeff(out, i, vec.at(i));
	return out;
}

auto Code::generate_h0() -> void {
	double tmp = q / 3.0;
	ZZ_pX h0_significant;
	SetCoeff(h0_significant, 0, lround(tmp));
	while (true) {
		try {
			h0 = generate_random_poly() + h0_significant;
			h0_inverse = InvMod(h0, modulus);
			break;
		} catch(const InvModErrorObject& ex) {
			continue;
		}
	}
}

auto Code::generate_h1() -> void {
	double tmp = q / 9.0;
	ZZ_pX h1_significant;
	SetCoeff(h1_significant, 0, lround(tmp));
	h1 = generate_random_poly() + h1_significant;
}

auto Code::generate_G() -> void {
	g = MulMod(h0_inverse, h1, modulus);
	G.SetDims(k, k);
	for (long i = 0; i < k; ++i) {
		for (long j = 0; j < k; ++j) {
			G.put(i, j, ZZ_p{coeff(g, j)});
		}
		g = MulByXMod(g, modulus);
	}

	// for transpose:
	SetCoeff(g_transpose, 0, ZZ_p{coeff(g, 0)});
	for (long i = 1; i < k; ++i)
		SetCoeff(g_transpose, i, ZZ_p{coeff(g, k - i)});
}


/*-----------*\
 PUBLIC
\*-----------*/


auto Code::calculate_syndrome(const vec_ZZ_p& message) -> vec_ZZ_p {
	throw "not yet implemented!";
	return message;
}

auto Code::encode_with_transpose(const vec_ZZ_p& message) const -> vec_ZZ_p {
	if (message.length() != k)
		throw invalid_argument("Code::encode_with_transpose: Given message is not of length k!");
	vec_ZZ_p encoded = VectorCopy(message, k);
	vec_ZZ_p coeffs;
	for (long i = 0; i < k; ++i) {
		coeffs = VectorCopy(g_transpose, k);
		g_transpose = MulByXMod(g_transpose, modulus);
		encoded.append(message * coeffs);
	}
	return encoded;
}

auto Code::encode(const vec_ZZ_p& message) const -> vec_ZZ_p {
	if (message.length() != k)
		throw invalid_argument("Code::encode: Given message is not of length k!");
	vec_ZZ_p encoded = VectorCopy(message, k);
	vec_ZZ_p encoded_right;
	mul(encoded_right, message, G);
	append(encoded, encoded_right);
	return encoded;
}

auto Code::decode(const vec_ZZ_p& message, const size_t max_iter) const -> vec_ZZ_p {
	if (message.length() != 2*k)
		throw invalid_argument("Code::decode: Given message is not of length 2*k!");
	// dummy code to suppress warnings
	size_t i = max_iter;
	++i;
	throw "Not yet implemented!";
}

auto Code::init_keys() -> void {
	generate_h0();
	generate_h1();
	generate_G();
}

auto Code::get_g() const -> const ZZ_pX& {
	return g;
}

auto Code::get_G() const -> const mat_ZZ_p& {
	return G;
}

auto Code::get_k() const -> long {
	return k;
}

auto Code::get_q() const -> long {
	return q;
}