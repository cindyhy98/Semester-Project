#include <cstdio>
#include <iostream>
#include "generate_pki.h"

namespace pki {
    EVP_PKEY* generate_key() {
        /* Allocate memory for the EVP_PKEY structure. */
        EVP_PKEY * pkey = EVP_PKEY_new();
        if(!pkey)
        {
            std::cerr << "Unable to create EVP_PKEY structure." << std::endl;
            return NULL;
        }

        /* Generate the RSA key and assign it to pkey. */
        RSA* rsa = RSA_new();
        BIGNUM* bne = BN_new();
        BN_set_word(bne, RSA_F4);
        RSA_generate_key_ex(rsa, 2048, bne, NULL);
//
        if(!EVP_PKEY_assign_RSA(pkey, rsa))
        {
            std::cerr << "Unable to generate 2048-bit RSA key." << std::endl;
            EVP_PKEY_free(pkey);
            return NULL;
        }

        /* The key has been generated, return it. */
        return pkey;
    }


    X509* generate_x509 (EVP_PKEY* pkey) {
        X509* x509 = X509_new();

        /* set a few parameters of the certificate. */

        /* certificate expiration date: 365 days from now (60s * 60m * 24h * 365d) */
        X509_gmtime_adj(X509_get_notBefore(x509), 0);
        X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);

        X509_set_pubkey(x509, pkey);

        /* set the name of the issuer to the name of the subject. */
        X509_NAME* name = X509_get_subject_name(x509);
        X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char*)"CH", -1, -1, 0);
        X509_NAME_add_entry_by_txt(name, "ST", MBSTRING_ASC, (unsigned char*)"", -1, -1, 0);
        X509_NAME_add_entry_by_txt(name, "L", MBSTRING_ASC, (unsigned char*)"", -1, -1, 0);
        X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char*)"", -1, -1, 0);
        X509_NAME_add_entry_by_txt(name, "OU", MBSTRING_ASC, (unsigned char*)"dev", -1, -1, 0);
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"127.0.0.1", -1, -1, 0);

        X509_set_issuer_name(x509, name);

        /* finally sign the certificate with the key. */
        X509_sign(x509, pkey, EVP_sha256());

        return x509;
    }


}

