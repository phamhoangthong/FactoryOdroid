//
// Created by phamh on 1/30/2018.
//

#ifndef FACTORYODROID_MCLIENT_H
#define FACTORYODROID_MCLIENT_H
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
///////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////
extern void* MCLIENT_THREAD(void *param);
///////////////////////////////////////////////////////////////////////////////////////////////////
class MClient {
#define SIZE_BUFFER 1024*4
public:
    MClient(int socket, string code);
    MClient();
    ~MClient();

private:
    int socket_tcp_ip;
    bool flag_run;
    string m_code;
    pthread_t m_thread;

public:
    void init(int socket, string code);
    int start();
    void stop();
    void run();
    void send(char* data,size_t length);
    string getCode();

    virtual void haveData(string code, char* m_data, size_t length) = 0;
    virtual void haveError(string code, string m_msg) = 0;
    virtual void sended(string code) = 0;
    virtual void disconnect(string code) = 0;
};
///////////////////////////////////////////////////////////////////////////////////////////////////
#endif //FACTORYODROID_MCLIENT_H
