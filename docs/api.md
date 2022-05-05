# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`Code`](#classCode) | [Code](#classCode) class is an instance of MDPC GF(p), holds matrices H and G and performs encoding and decoding.
`class `[`Protocol`](#classProtocol) | [Protocol](#classProtocol) class holds an instance of MDPC GF(p) cryptosystem and allows for encryption and decryption using this instance.
`class `[`Random`](#classRandom) | [Random](#classRandom) class is a singleton used to generate random integers, vectors and polynomials.
`struct `[`DecodeBounds`](#structDecodeBounds) | [DecodeBounds](#structDecodeBounds) struct holds values used in decoding.

# class `Code` 

[Code](#classCode) class is an instance of MDPC GF(p), holds matrices H and G and performs encoding and decoding.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Code`](#classCode_1ab060cc77a2e7dbd74df84e7774c51db1)`(unsigned q,unsigned k)` | Create a new instance of MDPC GF(p) and generate keys.
`public auto `[`init_keys`](#classCode_1a7319dea60fc127aba6408847f4efbf19)`()` | Generate keys.
`public auto `[`encode`](#classCode_1a2ac84bd2a4ba29af23a838386c0f9edd)`(const vector< unsigned > & vec)` | Encode a vector.
`public auto `[`decode`](#classCode_1a48dcaf1a4bdf68ec6ed556162ccdc50a)`(const vector< unsigned > & vec,unsigned num_iterations)` | Decode a vector.

## Members

#### `public  `[`Code`](#classCode_1ab060cc77a2e7dbd74df84e7774c51db1)`(unsigned q,unsigned k)` 

Create a new instance of MDPC GF(p) and generate keys.

If the parameter q is not a power of 2, one may expect suboptimal performance of the decoder. In such scenario the decoding will fail with higher frequency. This may be used to test the decoder's performance, however. Hence such setting of the q is not disallowed.

#### Parameters
* `q` Parameter q, ideally a power of 2. 

* `k` Parameter k, ideally a prime.

#### `public auto `[`init_keys`](#classCode_1a7319dea60fc127aba6408847f4efbf19)`()` 

Generate keys.

The private key is the matrix H. The public kes is the matrix G. Matrix H consists of circulant blocks H0 and H1. These are represented by polynomials h0 and h1 respectively. Matrix G consists of one identity matrix block and a one circulant block. The keys are generated as follows:* Randomly generate h0.

* Randomly generate h1 such that it is invertible:* Randomly generate h1 in Z_q/(x^k -1).

* Convert it to Q.

* Calculate the inverse in Q.

* Convert the inverse to Z_q/(x^k -1).

* Calculate second block of matrix G as -(h1_inv*h0).

**See also**: Random::poly_internal(unsigned k, unsigned add_to_first = 0)

#### `public auto `[`encode`](#classCode_1a2ac84bd2a4ba29af23a838386c0f9edd)`(const vector< unsigned > & vec)` 

Encode a vector.

The vector vec is encoded as vec*G. The resulting vector is of length 2*k and its first k positions are equal to vec. This is because the first block of G is identity matrix.

#### Parameters
* `vec` A vector in Z_q of length k. 

#### Returns
An encoded vectpr in Z_q of length 2*k.

#### `public auto `[`decode`](#classCode_1a48dcaf1a4bdf68ec6ed556162ccdc50a)`(const vector< unsigned > & vec,unsigned num_iterations)` 

Decode a vector.

Decoding the vector means finding the used error vector. In the beginning, current error vector value is set to 0 and the vec's syndrome is calculated. If it is zero, decoding successfully ends and zero initialized error vector is returned. Otherwise, the vector is decoded iteratively up to num_iterations iterations. In each:* The current error vector is updated using the decide method and transformed using the transform method.

* The syndrome of the current error vector is calculated.

* This syndrome is subtracted from vec's syndrome. If the result is a zero vector, the decoding successfully ends and the current error vector is returned. In case all iterations finnish and the error vector is not returned, the decoding is considered a failure.

#### Parameters
* `vec` A vector in Z_q of length 2*k. 

* `num_iterations` Number of iterations. 

#### Returns
On success, return found error vector.

# class `Protocol` 

[Protocol](#classProtocol) class holds an instance of MDPC GF(p) cryptosystem and allows for encryption and decryption using this instance.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  `[`Protocol`](#classProtocol_1a31704b7046e529d7947657eb142d1cef)`(unsigned q,unsigned k)` | Generate a new MDPC GF(p) instance with the given q and k settings.
`public auto `[`encrypt`](#classProtocol_1af17ceff9d90017d56e01d12b53b35237)`(const vector< unsigned > & plaintext,bool verbose)` | Encrypt a plaintext.
`public auto `[`decrypt`](#classProtocol_1af6216872f0ab2e83dcabd5f419160a4d)`(const vector< unsigned > & ciphertext,unsigned num_iterations,bool verbose)` | Decrypt a ciphertext.

## Members

#### `public inline  `[`Protocol`](#classProtocol_1a31704b7046e529d7947657eb142d1cef)`(unsigned q,unsigned k)` 

Generate a new MDPC GF(p) instance with the given q and k settings.

To achieve the best possible DFR value, q should be a power of 2 and k should be a prime. It is not required, however.

**See also**: [Code](#classCode)

#### Parameters
* `q` Parameter q, ideally a power of 2. 

* `k` Parameter k, ideally a prime.

#### `public auto `[`encrypt`](#classProtocol_1af17ceff9d90017d56e01d12b53b35237)`(const vector< unsigned > & plaintext,bool verbose)` 

Encrypt a plaintext.

Encryption is achieved as follows:* Plaintext is encoded using the generated [Code](#classCode) instance.

* An error vector is generated.

* Encoded plaintext is summed with the error vector. Encryption will fail if and only if the plaintext length is not k. Ciphertext is of length 2*k.

**See also**: [Code::encode(const vector<unsigned>& plaintext)](#classCode_1a2ac84bd2a4ba29af23a838386c0f9edd)

#### Parameters
* `plaintext` A vector of length k with values from Z_q. 

* `verbose` Whether to print error messages. 

#### Returns
On success, return ciphertext.

#### `public auto `[`decrypt`](#classProtocol_1af6216872f0ab2e83dcabd5f419160a4d)`(const vector< unsigned > & ciphertext,unsigned num_iterations,bool verbose)` 

Decrypt a ciphertext.

Decryption is achieved as follows:* Ciphertext is decoded using the generated [Code](#classCode) instance to obtain the error vector used in encryption.

* The error vector is subtracted from the ciphertext.

* Plaintext is read as the first k positions from ciphertext. Decryption will fail if:

* Ciphertext is not of length 2*k.

* Decoding fails.

**See also**: [Code::decode(const vector<unsigned>& ciphertext, unsigned num_iterations)](#classCode_1a48dcaf1a4bdf68ec6ed556162ccdc50a)

#### Parameters
* `ciphertext` A vector of length 2*k with values from Z_q. 

* `num_iterations` Number of iterations used in the decoding step. 

* `verbose` Whether to print error messages. 

#### Returns
On success, return plaintext.

# class `Random` 

[Random](#classRandom) class is a singleton used to generate random integers, vectors and polynomials.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Random`](#classRandom_1aa4e614c2379a0484774bed412c486d6d)`(const `[`Random`](#classRandom)` & r) = delete` | Class [Random](#classRandom) is a singleton, so copying is disallowed.

## Members

#### `public  `[`Random`](#classRandom_1aa4e614c2379a0484774bed412c486d6d)`(const `[`Random`](#classRandom)` & r) = delete` 

Class [Random](#classRandom) is a singleton, so copying is disallowed.

#### Parameters
* `r`

# struct `DecodeBounds` 

[DecodeBounds](#structDecodeBounds) struct holds values used in decoding.

These values are needed in each iteration of decoding. They can be calculated from the parameter q and do not change until this parameter changes. The values are calculated using the [set()](#structDecodeBounds_1aa8149cc21797147a031f238ffcd615fb) method. The values are uninitialized until it is called.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public unsigned `[`q_thirds`](#structDecodeBounds_1a6339b88c1dcc398fa9e509130220f88b) | 
`public double `[`q_2`](#structDecodeBounds_1aee77db89774749a29f4941eafc84ac0f) | 
`public double `[`q_6`](#structDecodeBounds_1a3a57c2e80eec1799752d19843d1a6e4c) | 
`public double `[`q_18`](#structDecodeBounds_1aab6633faf8e8ac9403cb35fbbb08c1c5) | 
`public unsigned `[`b1`](#structDecodeBounds_1a122a65262ff1b15be89a34c951fadc0d) | 
`public unsigned `[`b2`](#structDecodeBounds_1a50f94bd66d8756d25d79205db798fa1d) | 
`public unsigned `[`b3`](#structDecodeBounds_1aa0189bb033223cffac835800ccf51311) | 
`public unsigned `[`b4`](#structDecodeBounds_1a99e8c0c9e461359ecabcd8151113e838) | 
`public unsigned `[`b5`](#structDecodeBounds_1a9c2372787104c269c4b91e315607be9d) | 
`public unsigned `[`b6`](#structDecodeBounds_1abf40150cff4a2e633a522ac2dddbc50f) | 
`public unsigned `[`b7`](#structDecodeBounds_1af44007aba84abf58e9637b7d85ff2f40) | 
`public unsigned `[`b8`](#structDecodeBounds_1a43b4e430eed89bd8d023699efc262712) | 
`public  `[`DecodeBounds`](#structDecodeBounds_1ad49c95f5df74c177f02cc83ab741ee29)`() = default` | 
`public auto `[`set`](#structDecodeBounds_1aa8149cc21797147a031f238ffcd615fb)`(unsigned q)` | Calculate the values.

## Members

#### `public unsigned `[`q_thirds`](#structDecodeBounds_1a6339b88c1dcc398fa9e509130220f88b) 

#### `public double `[`q_2`](#structDecodeBounds_1aee77db89774749a29f4941eafc84ac0f) 

#### `public double `[`q_6`](#structDecodeBounds_1a3a57c2e80eec1799752d19843d1a6e4c) 

#### `public double `[`q_18`](#structDecodeBounds_1aab6633faf8e8ac9403cb35fbbb08c1c5) 

#### `public unsigned `[`b1`](#structDecodeBounds_1a122a65262ff1b15be89a34c951fadc0d) 

#### `public unsigned `[`b2`](#structDecodeBounds_1a50f94bd66d8756d25d79205db798fa1d) 

#### `public unsigned `[`b3`](#structDecodeBounds_1aa0189bb033223cffac835800ccf51311) 

#### `public unsigned `[`b4`](#structDecodeBounds_1a99e8c0c9e461359ecabcd8151113e838) 

#### `public unsigned `[`b5`](#structDecodeBounds_1a9c2372787104c269c4b91e315607be9d) 

#### `public unsigned `[`b6`](#structDecodeBounds_1abf40150cff4a2e633a522ac2dddbc50f) 

#### `public unsigned `[`b7`](#structDecodeBounds_1af44007aba84abf58e9637b7d85ff2f40) 

#### `public unsigned `[`b8`](#structDecodeBounds_1a43b4e430eed89bd8d023699efc262712) 

#### `public  `[`DecodeBounds`](#structDecodeBounds_1ad49c95f5df74c177f02cc83ab741ee29)`() = default` 

#### `public auto `[`set`](#structDecodeBounds_1aa8149cc21797147a031f238ffcd615fb)`(unsigned q)` 

Calculate the values.

#### Parameters
* `q` Parameter q of the MDPC GF(p) instance.

Generated by [Moxygen](https://sourcey.com/moxygen)