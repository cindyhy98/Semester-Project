#include "message.h"

using namespace std;
namespace message {

    struct SubmitMsg m;

    void GenerateMsgHash(struct SubmitMsg* m, unsigned char* hash){

        // Convert struct SubmitMsg to char array
        char mChar[sizeof(SubmitMsg)+1];
        memcpy(mChar, &m, sizeof(SubmitMsg));

//    unsigned char hash[SHA256_DIGEST_LENGTH]; -> should be passed into the function
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, mChar, sizeof(mChar));
        SHA256_Final(hash, &sha256);
    }
}

