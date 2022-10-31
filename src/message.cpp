#include "message.h"

using namespace std;
namespace message {

    struct MsgSubmit m;

    void GenerateMsgHash(struct MsgSubmit* m, unsigned char* hash){

        // Convert struct MsgSubmit to char array
        char mChar[sizeof(MsgSubmit)+1];
        memcpy(mChar, &m, sizeof(MsgSubmit));

//    unsigned char hash[SHA256_DIGEST_LENGTH]; -> should be passed into the function
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, mChar, sizeof(mChar));
        SHA256_Final(hash, &sha256);
    }
}

