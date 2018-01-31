//
// Created by phamh on 1/30/2018.
//

#ifndef FACTORYODROID_MTCP_H
#define FACTORYODROID_MTCP_H
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "MClient.h"
#include "vector"
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <string.h>
///////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////
class MTCP {
#define LENGTH_CODE 10
public:
    MTCP();
    ~MTCP();

private:
    bool flag_run;
    pthread_t m_thread;
    int m_port;
    int m_socket;
    struct sockaddr_in server_address;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    class MyClient : public MClient{
    public:
        MTCP* m_parent;
        ///////////////////////////////////////////////////////////////////////////////////////////
        MyClient(int socket, string code, MTCP* &parent);
        void haveData(string code, char* data, size_t length);
        void haveError(string code, string msg);
        void sended(string code);
        void disconnected(string code);
        ///////////////////////////////////////////////////////////////////////////////////////////
    };
    ///////////////////////////////////////////////////////////////////////////////////////////////
    vector<MyClient> m_clients;
public:
    int start();
    void stop();
    void run();
    void init(int port);

    int disconnectClient(string code);

    virtual void haveDataFromClient(string code, char* data, size_t length) = 0;
    virtual void haveErrorFromClient(string code, string msg) = 0;
    virtual void sendedDataToClient(string code) = 0;
    virtual void disconnectedClient(string code) = 0;

    virtual void haveStatusServer(string msg) = 0;
    virtual void haveErrorServer(string msg) = 0;
private:
    int indexClient(string code);
    int connectSocket();
};
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif //FACTORYODROID_MTCP_H
