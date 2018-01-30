//
// Created by phamh on 1/30/2018.
//

#ifndef FACTORYODROID_MSERIALPORT_H
#define FACTORYODROID_MSERIALPORT_H

#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <vector>
#include <strings.h>
#include <pthread.h>

using namespace std;

void* MY_THREAD_SERIALPORT(void * parameter);

class MSerialPort {
#define M_BUFFER_SIZE 4096
public:
    MSerialPort() {
        flag_run = false;
        m_buffer = new char[M_BUFFER_SIZE];
    }

    ~MSerialPort() {
        delete[] m_buffer;
    }
private:
    int fd;
    bool flag_run;
    char* m_buffer;
    pthread_t m_thread;
public:
    int connect(const char* port, int speed) {
        if(fd > 0) {
            close(fd);
        }
        fd = open(port, O_RDWR | O_NOCTTY |O_NDELAY);
        if(fd < 0) {
            return -1;
        }
        fcntl(fd,F_SETFL,0);
        struct termios my_setting;
        cfsetispeed(&my_setting, B9600);
        cfsetospeed(&my_setting, B9600);
        my_setting.c_cflag &= ~PARENB;
        my_setting.c_cflag &= ~CSTOPB;
        my_setting.c_cflag &= ~CSIZE;
        my_setting.c_cflag |= CS8;
        my_setting.c_cflag &= ~CRTSCTS;
        tcsetattr(fd, TCSANOW, &my_setting);
        return 0;
    }

    int sendData(char* data, size_t length) {
        if(fd < 0) {
            return -1;
        }
        write(fd, data, length);
        return 0;
    }

    void run() {
        int m_size = 0;
        while(flag_run) {
            m_size = read(fd, m_buffer, M_BUFFER_SIZE);
            if(m_size > 0) {
                haveData(m_buffer,m_size);
            }
            usleep(1);
        }
    }

    int start() {
        int status;
        flag_run = true;
        status = pthread_create(&m_thread, NULL, MY_THREAD_SERIALPORT, this);
        if(status) {
            return -1;
        } else {
            return 0;
        }
    }

    void stop() {
        if(flag_run) {
            flag_run = false;
            pthread_join(m_thread, NULL);
        }
    }

    virtual void haveData(char* buf, size_t size) = 0;
};

void* MY_THREAD_SERIALPORT(void * parameter) {
    MSerialPort* mSerialPort = (MSerialPort*)parameter;
    mSerialPort->run();
}


#endif //FACTORYODROID_MSERIALPORT_H
