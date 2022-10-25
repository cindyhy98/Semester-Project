#ifndef ACCOUNTABLE_CONFIRMER_GENERATE_PKI_H
#define ACCOUNTABLE_CONFIRMER_GENERATE_PKI_H

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

namespace pki{
    EVP_PKEY* generate_key();
    X509* generate_x509 (EVP_PKEY* pkey);
}

#endif //ACCOUNTABLE_CONFIRMER_GENERATE_PKI_H
