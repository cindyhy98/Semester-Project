#include <boost/format.hpp>

#include "core.h"

using namespace std;

namespace core {

    queue<string> recvRawMessage;
    /* Reliable Broadcast */

    void ParseMessage(struct Peer* p) {

        while(!p->detectConflict) {
            while(!recvRawMessage.empty()) {
                string message = recvRawMessage.front();

                message::RBMessage* recvMsg = new(message::RBMessage);
                int start = 0; int end = (int)message.find("::");
                string type_str(message.substr(start, end));


                if (atoi(type_str.c_str()) != message::submit &&
                    atoi(type_str.c_str()) != message::aggSign ) {

                    recvMsg->type = atoi(type_str.c_str());

                    start = end+2; end = (int)message.find("::", start);
                    string pid_str(message.substr(start, end-start));
                    recvMsg->pid = atoi(pid_str.c_str());

                    start = end+2; end = (int)message.find("::", start);
                    string content_str(message.substr(start, end-start));
                    recvMsg->content = content_str;

                    switch (recvMsg->type) {
                        case message::none:
                            printf("Something goes wrong\n");
                            break;
                        case message::send:

                            DeliverSend(p, recvMsg);
                            break;
                        case message::echo:

                            DeliverEcho(p, recvMsg);
                            break;
                        case message::ready:

                            DeliverReady(p, recvMsg);
                            break;
                    }
                } else {
                    string trim_message = message.substr(end+2, message.length());

                    switch (atoi(type_str.c_str())) {
                        case message::submit:
                            ParseSubmitMessage(p, trim_message);
                            break;
                        case message::aggSign:
                            ParseAggSignature(p, trim_message);
                            break;
                    }
                }


                recvRawMessage.pop();
                delete(recvMsg);
            }

        }
    }

    void Broadcast(struct Peer* p, struct message::RBMessage* msg) {
        /* Need to clear the serializeMsg everytime before sending */
        vector<byte> serializeMsg;
        serializeMsg.clear();

        /* Serialize message */
        auto pre = (boost::format("%1%::%2%::%3%::") % msg->type % msg->pid % msg->content).str();
        for (char i : pre) serializeMsg.push_back(byte(i));

        string message(reinterpret_cast<const char*>(serializeMsg.data()), serializeMsg.size());
        message += '\n';

        /* Send message to all the peers */
        p->clients[p->id - 1].Post(message);
        usleep(100000);
        return;

    }

    void BroadcastSend(struct Peer* p) {
        /* Define the type of message */
        p->rbMsg.type = message::send;

        /* First, check if this recvMsg is sent by yourself */
        if(p->rbMsg.pid != p->id) return;

        Broadcast(p, &p->rbMsg);
    }

    void DeliverSend(struct Peer* p, struct message::RBMessage* recvMsg) {

        /* Check if the sentecho is still false */
        if(p->rb.sentecho == true) return;

        p->rb.sentecho = true;

        /* Change the recvMsg type to echo and broadcast it to everynody */
        recvMsg->type = message::echo;
        Broadcast(p, recvMsg);

    }

    void DeliverEcho(struct Peer* p, struct message::RBMessage* recvMsg) {
        /* Change the recvMsg type to ready
         * and store recvMsg to this peer's ReliableBroadcastInstance */
        recvMsg->type = message::ready;
        p->rb.echos.push_back(*recvMsg);

        if (p->rb.echos.size() < 2 * NUMBER_OF_FAULTY_PROCESSES + 1) return;

        if (p->rb.sentready == true) return;

        p->rb.sentready = true;

        /* broadcast this ready to everynody */
        Broadcast(p, recvMsg);

    }

    void DeliverReady(struct Peer* p, struct message::RBMessage* recvMsg) {
        /* Store the received ready to this peer's ReliableBroadcastInstance */
        p->rb.readys.push_back(*recvMsg);

        if (p->rb.readys.size() <= NUMBER_OF_FAULTY_PROCESSES ) return;
        if (p->rb.sentready == true) return;

        p->rb.sentready = true;

        /* Broadcast this ready to everynody */
        Broadcast(p, recvMsg);

    }

    void CheckIfReady(struct Peer* p) {

        while(true) {
            /* Check if receive enough ready */
            if (p->rb.readys.size() > 2 * NUMBER_OF_FAULTY_PROCESSES && !p->rb.delivered) {
                p->rb.delivered = true;
                break;
            }

        }
        printf("[DeliverReady] [%d] Receive enough ready -> can start to deliver!!\n", p->id);

    }

    /* Accountable Confirmer */

    /* Return 1 if found, Return 0 if not found */
    int FindPid(vector<int> pidVector, int elem){
        vector<int>::iterator it = find(pidVector.begin(), pidVector.end(), elem);
        if (it != pidVector.end())   return 1;
        else    return 0;

    }

    void ShareSign(struct Peer* p) {

        auto msg = to_string(p->msg.value);
        /* Create a partial signature for the submit value (p->msg.value) */
        accountable_confirmer_bls::Sign(&p->keyPair, &p->msg.sig, msg.c_str());
    }

    /* Return 1 if it is valid, Return 0 if not valid */
    int ShareVerify(struct message::SubmitMsg* recvMsg) {

        auto msg = to_string(recvMsg->value);

        /* Verify the partial signature of the submit value */
        if (accountable_confirmer_bls::Verify(&recvMsg->pub, &recvMsg->sig, msg.c_str())) {
            printf("[ShareVerify] Successful\n");
            return 1;
        } else {
            printf("[ShareVerify] Fail\n");
            return 0;
        }

    }

    void SubmitMsgVerify(struct Peer* p, struct message::SubmitMsg* recvMsg) {

        int id = recvMsg->pid;

        /* Authenticate the whole submit message */
        if (ShareVerify(recvMsg)) {
            if (p->ac.value == recvMsg->value) {
                printf("[SubmitMsgVerify] Recv value(%d) of [%d] is equal to it's submit value\n",recvMsg->value, id);
                if (!FindPid(p->ac.from, id)) {
                    printf("[SubmitMsgVerify] update accountable confirmer\n");

                    /* This recvMsg is verified, need to store it in the AccountableConfirmer */
                    p->ac.from.push_back(id);
                    p->ac.partialSignature.push_back(*recvMsg);
                    CheckRecvMsg(p);
                } else {
                    printf("[SubmitMsgNotVerify] peer [%d] has already sent message\n", recvMsg->pid);
                }
            } else {
                printf("[SubmitMsgNotVerify] Recv value(%d) of [%d] is not equal to it's submit value(%d)\n",recvMsg->value, id, p->ac.value);
            }
        }

    }



    /* Combine the received partial signatures into a aggregate signature */
    void GenerateAggSignature(struct Peer* p) {

        int submitValue;
        vector<blsSignature> sigVec;
        vector<blsPublicKey> pubVec;

        for(auto & elem : p->ac.partialSignature){
            sigVec.push_back(elem.sig);
            pubVec.push_back(elem.pub);
            submitValue = elem.value;
        }

        auto msg = to_string(submitValue).c_str();

        blsSignature aggSig;

        /* Create an aggregate signature for the collected partial signatures */
        accountable_confirmer_bls::AggSign(&aggSig, &sigVec[0], strlen(msg));

        p->aggSignMsg = {.type = message::aggSign, .pid = p->id, .value = submitValue, .aggSig = aggSig};
    }

    void BroadcastSubmitMessage(struct Peer* p, int to) {
        /* Need to clear the serializeMsg everytime before sending */
        vector<byte> serializeMsg;
        serializeMsg.clear();


        /* Serialize blsSignature */
        byte bufSig[128];
        auto sig_sz = blsSignatureSerialize(bufSig, 128, &p->msg.sig);
        vector<byte> vSig; vSig.reserve(sig_sz);
        for (int i = 0; i < sig_sz; i++) vSig.push_back(bufSig[i]);

        /* Serialize blsPublicKey */
        byte bufPub[128];
        auto pub_sz = blsPublicKeySerialize(bufPub, 128, &p->msg.pub);
        vector<byte> vPub; vPub.reserve(pub_sz);
        for (int i = 0; i < pub_sz; i++)  vPub.push_back(bufPub[i]);

        /* Serialize Submit message */
//        string message_type = "submit";
        auto pre = (boost::format("%1%::%2%::%3%::%4%::%5%::") % p->msg.type % to % p->msg.pid % p->msg.value % sig_sz ).str();
        for (char i : pre) serializeMsg.push_back(byte(i));
        serializeMsg.insert(serializeMsg.end(), vSig.begin(), vSig.end());
        auto mid = (boost::format("::%1%::") % pub_sz ).str();
        for (char i : mid) serializeMsg.push_back(byte(i));
        serializeMsg.insert(serializeMsg.end(), vPub.begin(), vPub.end());
        auto end = (boost::format("::") ).str();
        for (char i : end) serializeMsg.push_back(byte(i));

        /* Send the serialized message */
        string submit_message(reinterpret_cast<const char*>(serializeMsg.data()), serializeMsg.size());
        submit_message += '\n';

        /* First we need to go through reliable broadcast */

        /* Init the rbMsg */
        p->rbMsg = {.type = message::none, .pid = p->id, .content = submit_message};
        BroadcastSend(p);

        while(!p->rb.delivered) {
            // keep waiting
        }
        printf("[%d] You can start to deliver!!\n", p->id);

        /* After going through reliable broadcast, each peer can finally start sending message */

        printf("[BroadcastSubmitMessage] client [%d] Send submit message to [%d]\n",p->id, to);
        p->clients[p->id - 1].Post(submit_message);
        usleep(100000);
        return;

    }

    void BroadcastAggregateSignature(struct Peer* p, int to) {
        /* Need to clear the serializeAggSign everytime before sending */
        vector<byte> serializeAggSign;
        serializeAggSign.clear();

        /* Serialize blsSignature (Aggregate Signature) */
        byte bufSig[128];
        auto sig_sz = blsSignatureSerialize(bufSig, 128, &p->aggSignMsg.aggSig);
        vector<byte> vSig; vSig.reserve(sig_sz);
        for (int i = 0; i < sig_sz; i++) vSig.push_back(bufSig[i]);

        /* Serialize SubmitAggSign */
//        string message_type = "aggSign";
        auto pre = (boost::format("%1%::%2%::%3%::%4%::%5%::") % p->aggSignMsg.type % to % p->aggSignMsg.pid % p->aggSignMsg.value % sig_sz ).str();
        for (char i : pre) serializeAggSign.push_back(byte(i));
        serializeAggSign.insert(serializeAggSign.end(), vSig.begin(), vSig.end());
        auto end = (boost::format("::") ).str();
        for (char i : end) serializeAggSign.push_back(byte(i));

        /* Send the serialized message */
        string aggSign(reinterpret_cast<const char*>(serializeAggSign.data()), serializeAggSign.size());
        aggSign += '\n';

        printf("[BroadcastAggregateSignature] [%d] Broadcast aggregate signature to peer\n", p->id);
        p->clients[p->id - 1].Post(aggSign);
        usleep(100000);
        return;
    }

    void ParseAggSignature(struct Peer* p, string message) {
        message::SubmitAggSign* recvAggSign = new(message::SubmitAggSign);
        int start = 0; int end = (int)message.find("::");
        string to_str(message.substr(start, end));
        int to = atoi(to_str.c_str());

        if (to == 0 || to == p->id) {
            /* Keep parsing! */
        } else {
            /* Ignore the message for others */
            printf("Ignore message not send to you\n");
            return;
        }

        start = end+2; end = (int)message.find("::", start);
        string pid_str(message.substr(start, end-start));
        recvAggSign->pid = atoi(pid_str.c_str());

        start = end+2; end = (int)message.find("::", start);
        string value_str(message.substr(start, end-start));
        recvAggSign->value = atoi(value_str.c_str());

        start = end+2; end = (int)message.find("::", start);
        string sig_sz_str(message.substr(start, end-start));
        int sig_sz = atoi(sig_sz_str.c_str());

        start = end+2; end = (int)message.find("::", start);
        string sig_str(message.substr(start, end-start));

        byte sig_byte[sig_str.length()];
        memcpy(sig_byte, sig_str.data(), sig_sz);
        blsSignatureDeserialize(&recvAggSign->aggSig, &sig_byte, sig_sz);

        p->ac.obtainedAggSignature.push_back(*recvAggSign);
        CheckRecvAggSignature(p);
        delete(recvAggSign);

    }

    void ParseSubmitMessage(struct Peer* p, string message) {

        message::SubmitMsg* recvMsg = new(message::SubmitMsg);
        int start = 0; int end = (int)message.find("::");
        string to_str(message.substr(start, end));
        int to = atoi(to_str.c_str());


        if (to == 0 || to == p->id) {
            /* Keep parsing! */
        } else {
            /* Ignore the message for others */
            printf("Ignore message not send to you\n");
            return;
        }

        start = end+2; end = (int)message.find("::", start);
        string pid_str(message.substr(start, end-start));
        recvMsg->pid = atoi(pid_str.c_str());

        start = end+2; end = (int)message.find("::", start);
        string value_str(message.substr(start, end-start));
        recvMsg->value = atoi(value_str.c_str());

        start = end+2; end = (int)message.find("::", start);
        string sig_sz_str(message.substr(start, end-start));
        int sig_sz = atoi(sig_sz_str.c_str());

        start = end+2; end = (int)message.find("::", start);
        string sig_str(message.substr(start, end-start));

        byte sig_byte[sig_str.length()];
        memcpy(sig_byte, sig_str.data(), sig_sz);
        blsSignatureDeserialize(&recvMsg->sig, &sig_byte, sig_sz);

        start = end+2; end = (int)message.find("::", start);
        string pub_sz_str(message.substr(start, end-start));
        int pub_sz = atoi(pub_sz_str.c_str());

        start = end+2; end = (int)message.find("::", start);
        string pub_str(message.substr(start, end-start));

        byte pub_byte[pub_str.length()];
        memcpy(pub_byte, pub_str.data(), pub_sz);
        blsPublicKeyDeserialize(&recvMsg->pub, &pub_byte, pub_sz);

        SubmitMsgVerify(p, recvMsg);
        delete(recvMsg);

    }

//    void ParseMessage(struct Peer* p) {
//
//        while(!p->detectConflict && p->rb.delivered) {
//
//            while(!recvRawMessage.empty()) {
//                string message = recvRawMessage.front();
//                int start = 0; int end = (int)message.find("::");
//                string message_type(message.substr(start, end));
//                string trim_message = message.substr(end+2, message.length());
//
//                if (message_type == "submit") {
//                    printf("parsing Submit Message\n");
//                    ParseSubmitMessage(p, trim_message);
//                } else if(message_type == "aggSign") {
//                    printf("parsing Agg Sign\n");
//                    ParseAggSignature(p, trim_message);
//                } else {
//                    // couldn't parse
//                }
//
//                recvRawMessage.pop();
//            }
//
//        }
//    }

    void CheckRecvMsg(struct Peer* p){

        if (p->ac.from.size() >= NUMBER_OF_PROCESSES - NUMBER_OF_FAULTY_PROCESSES) {
            printf("[CheckRecvMsg] [%d] Receiving enough messages -> progress to Confirm phase\n", p->id);
            Confirm(p);
        }

    }

    void CheckRecvAggSignature(struct Peer* p){

        if (p->ac.obtainedAggSignature.size() >= 2) {
            printf("[CheckRecvAggSignature] receive enough aggSignatures -> progress to detect phase\n");
            DetectConflictAggSignature(p);
        }

    }

    void InitClient(struct Peer* p, int index, int portNumber){

        p->clients[index].Init("localhost", portNumber);
        p->clients[index].OnMessage = [] (const std::string& message) {
            recvRawMessage.push(message);
        };
        thread t( [&p, &index] () { p->clients[index].Run(); for(;;);});
        p->clientThread.emplace_back(move(t));
        usleep(100000);
    }


    /* Main functionality */

    void InitPeer(struct Peer* p, int id, int totalPeers) {

        p->id = id;

        /* Generate blsPublicKey and blsSecretKey for SubmitMsg Enc/Dec */
        accountable_confirmer_bls::Init();
        accountable_confirmer_bls::KeyGen(&p->keyPair);

        /* Clear the receiving queue */
        while (!recvRawMessage.empty()) recvRawMessage.pop();

        p->recvMsgFlag = false;
        p->detectConflict = false;

        /* Init accountable confirmer */
        AccountableConfirmer();

        /* Init reliable broadcast instance */
        ReliableBroadcast();


        /* Init Clients */
        for (int i = 0; i < totalPeers; i++) {
            InitClient(p, i, DEFAULT_PORT_NUMBER+i+1);
        }


        /* Start listening to the received message*/
        thread runParse(ParseMessage, p);
        p->recvThread.emplace_back(move(runParse));

        thread runDeliver(CheckIfReady, p);
        p->recvThread.emplace_back(move(runDeliver));


//        thread runParse(ParseMessage, p);
//        p->recvThread.emplace_back(move(runParse));


        printf("[InitPeer] [%d] Init \n", p->id );
    }

    void Submit(struct Peer* p, int value, int to) {



        /* Init the submitMsg */
        p->msg.type = message::submit;
        p->msg.pid = p->id;
        p->msg.value = value;
        p->msg.pub = p->keyPair.pub;

        /* Init the partial signature for the submit value */
        ShareSign(p);

        /* Store this peer's value to the AccountableConfirmer */
        p->ac.value = value;

        /* Broadcast the submitMsg to everyone */
        BroadcastSubmitMessage(p, to);

        printf("[Submit] [%d] submit value %d \n", p->id, value);
    }

    void Confirm(struct Peer* p) {

        /* Confirm in the AccountableConfirmer */
        p->ac.confirm = true;

        /* Generate the aggregate signature and store it in p */
        GenerateAggSignature(p);

        /* Broadcast the aggregate signature to everyone */
        BroadcastAggregateSignature(p, 0);

        /* This flag will make the peer start sending aggregate Signature*/
        p->recvMsgFlag = true;

        printf("[Confirm] [%d] Confirmed\n", p->id);
    }

    void DetectConflictAggSignature(struct Peer* p) {

        message::SubmitAggSign tmp = p->ac.obtainedAggSignature.front();

        for(vector<message::SubmitAggSign>::size_type i = 1; i != p->ac.obtainedAggSignature.size(); i++) {
            printf("[DetectConflictAggSignature] [%d] Detecting....\n", p->id);

            if(!(p->ac.obtainedAggSignature[i] == tmp) && p->ac.confirm){
                printf("[DetectConflictAggSignature] [%d] REACH THE PROOF!!!\n", p->id);
                p->detectConflict = true;
                break;
            }
        }

    }

    void Close(struct Peer* p) {

        for (auto &thr : p->recvThread) {
            thr.detach();
        }

        for (auto &thr : p->clientThread) {
            thr.detach();
        }

        printf("[Close] Successfully\n");
    }

}

