#ifndef ACCOUNTABLE_CONFIRMER_BLS_H
#define ACCOUNTABLE_CONFIRMER_BLS_H

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/* Import BLS */
#define BLS_ETH
#include <mcl/bn_c384_256.h>
#include <bls/bls.h>



namespace accountable_confirmer_bls {

    /*
    struct PublicKey {
        blsPublicKey key;   // Public Key

        template<class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & key.v.x.d[j];
            }
            for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                ar & key.v.y.d[j];
            }
            for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                ar & key.v.z.d[j];
            }
        }
    };

    struct SecretKey {
        blsSecretKey key;   // Private key
    };

    struct Signature {
        blsSignature key;

        template<class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            for (int i = 0; i < 2; i++){
//                ar & sig.v.x.d[i];
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & key.v.x.d[i].d[j];
                }

            }
            for (int i = 0; i < 2; i++){
//                ar & sig.v.y.d[i];
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & key.v.y.d[i].d[j];
                }

            }
            for (int i = 0; i < 2; i++){
//                ar & sig.v.z.d[i];
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & key.v.z.d[i].d[j];
                }

            }
        }
    };
    */

    struct Key {
        blsSecretKey sec;   // Private key
        blsPublicKey pub;   // Public Key
    };


    void Init();

    void KeyGen(Key* k);

    void Sign(Key* k, blsSignature *sig, char msg[]);

    /* return 1 if it is valid else 0 */
    int Verify(blsPublicKey *pub, blsSignature *sig, char msg[]);

    void AggSign(blsSignature *aggSig, const blsSignature *sigVec, mclSize n);

    /* return 1 if it is valid else 0 */
    int FastAggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, mclSize n,  const void *msg, mclSize msgSize);

    /* return 1 if it is valid else 0 */
    int AggSignVerify(const blsSignature *sig, const blsPublicKey *pubVec, const void *msgVec, mclSize msgSize, mclSize n);
}
#endif //ACCOUNTABLE_CONFIRMER_BLS_H
