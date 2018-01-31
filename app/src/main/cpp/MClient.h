//
// Created by phamh on 1/30/2018.
//

#ifndef FACTORYODROID_MCLIENT_H
#define FACTORYODROID_MCLIENT_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>

using namespace std;

extern void* MCLIENT_THREAD(void *param);

class MClient {
#define SIZE_BUFFER 1024*4
public:
    MClient();
    ~MClient();

private:
    int socket_tcp_ip;
    bool flag_run;
    string code;
    pthread_t m_thread;

public:
    int start();
    void stop();
    void run();
    void send(char* data,size_t length);

    virtual void haveData(string code, char* m_data, size_t length) = 0;
    virtual void haveError(string m_code, string m_msg) = 0;
    virtual void sended(string m_code) = 0;
};


#endif //FACTORYODROID_MCLIENT_H
