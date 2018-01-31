//
// Created by phamh on 1/30/2018.
//

#ifndef FACTORYODROID_MTCP_H
#define FACTORYODROID_MTCP_H

#include "MClient.h"

using namespace std;

class MTCP {
public:
    MTCP();
    ~MTCP();

private:
    bool flag_run;
    pthread_t m_thread;

public:
    int start();
    void stop();
    void run();
};


#endif //FACTORYODROID_MTCP_H
