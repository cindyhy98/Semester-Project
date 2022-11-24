#ifndef ACCOUNTABLE_CONFIRMER_BLS_H
#define ACCOUNTABLE_CONFIRMER_BLS_H

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/* Import BLS */
#define BLS_ETH
#include <mcl/bn_c384_256.h>
#include <bls/bls.h>



namespace accountable_confirmer_bls {

    struct PublicKey {
        blsPublicKey pub;   // Public Key

        template<class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & pub.v.x.d[j];
            }
            for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                ar & pub.v.y.d[j];
            }
            for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                ar & pub.v.z.d[j];
            }
//            ar & pub.v.x.d & pub.v.y.d & pub.v.z.d;
        }
    };

    struct SecretKey {
        blsSecretKey sec;   // Private key
    };

    struct Signature {
        blsSignature sig;

        template<class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            for (int i = 0; i < 2; i++){
//                ar & sig.v.x.d[i];
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & sig.v.x.d[i].d[j];
                }

            }
            for (int i = 0; i < 2; i++){
//                ar & sig.v.y.d[i];
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & sig.v.y.d[i].d[j];
                }

            }
            for (int i = 0; i < 2; i++){
//                ar & sig.v.z.d[i];
                for (int j = 0; j < MCLBN_FP_UNIT_SIZE; j++){
                    ar & sig.v.z.d[i].d[j];
                }

            }
        }
    };

    struct Key {
        SecretKey sec;   // Private key
        PublicKey pub;   // Public Key
    };

    void Init();

    void KeyGen(Key* k);

    void Sign(Key* k, Signature *sig, char msg[]);

    /* return 1 if it is valid else 0 */
    int Verify(PublicKey pub, Signature sig, char msg[]);

    void AggSign(Signature *aggSig, const Signature *sigVec, mclSize n);

    /* return 1 if it is valid else 0 */
    int FastAggSignVerify(const Signature *sig, const PublicKey *pubVec, mclSize n,  const void *msg, mclSize msgSize);

    /* return 1 if it is valid else 0 */
    int AggSignVerify(const Signature *sig, const PublicKey *pubVec, const void *msgVec, mclSize msgSize, mclSize n);
}
#endif //ACCOUNTABLE_CONFIRMER_BLS_H
