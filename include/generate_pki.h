#ifndef ACCOUNTABLE_CONFIRMER_GENERATE_PKI_H
#define ACCOUNTABLE_CONFIRMER_GENERATE_PKI_H

#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

namespace rsapki{
    EVP_PKEY* generate_key();
    X509* generate_x509 (EVP_PKEY* pkey);
//    EVP_PKEY* generate_rsa_key(unsigned int key_bits);
//    EVP_PKEY* load_key(std::string key_file_type, std::string key_file);
}

#endif //ACCOUNTABLE_CONFIRMER_GENERATE_PKI_H
