///////////////////////////////////////////////////////////////////////////////////////////////////
#include <jni.h>
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <vector>
///////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////
#define M_BUFFER_SIZE 256
///////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
JNIEXPORT jint

JNICALL
Java_com_example_phamh_factoryodroid_RS485_getIdRS485(
        JNIEnv *env,
        jobject parent,
        jstring port,
        jint speed) {
    const char *portString = env->GetStringUTFChars(port, JNI_FALSE);
    int m_id = open(portString, O_RDWR | O_NOCTTY | O_NDELAY);
    if (m_id < 0) {
        return -1;
    }
    fcntl(m_id, F_SETFL, 0);
    struct termios my_setting;
    switch (speed) {
        case 9600:
            cfsetispeed(&my_setting, B9600);
            cfsetospeed(&my_setting, B9600);
            break;
        case 115200:
            cfsetispeed(&my_setting, B115200);
            cfsetospeed(&my_setting, B115200);
            break;
        default:
            return -2;
    }
    my_setting.c_cflag &= ~PARENB;
    my_setting.c_cflag &= ~CSTOPB;
    my_setting.c_cflag &= ~CSIZE;
    my_setting.c_cflag |= CS8;
    my_setting.c_cflag &= ~CRTSCTS;
    my_setting.c_cc[VMIN] = 1;
    my_setting.c_cc[VTIME] = 20;
    tcsetattr(m_id, TCSANOW, &my_setting);
    return m_id;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
JNIEXPORT jbyteArray

JNICALL
Java_com_example_phamh_factoryodroid_RS485_readDataRS485(
        JNIEnv *env,
        jobject parent,
        jint id) {
    int m_size = 0;
    char *m_buffer = new char[M_BUFFER_SIZE];
    m_size = read(id, m_buffer, M_BUFFER_SIZE);
    if (m_size > 0) {
        jbyte *m_buf = new jbyte[m_size];
        for (int i = 0; i < m_size; i++) {
            m_buf[i] = (jbyte) (m_buffer[i]);
        }
        jbyteArray m_return = env->NewByteArray(m_size);
        env->SetByteArrayRegion(m_return, 0, m_size, m_buf);
        delete[](m_buf);
        delete[](m_buffer);
        return m_return;
    } else {
        delete[](m_buffer);
        return env->NewByteArray(0);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
JNIEXPORT jint

JNICALL
Java_com_example_phamh_factoryodroid_RS485_sendDataRS485(
        JNIEnv *env,
        jobject parent,
        jint id,
        jbyteArray data) {
    int m_size = env->GetArrayLength(data);
    jbyte *m_buf = new jbyte[m_size];
    env->GetByteArrayRegion(data, 0, m_size, m_buf);
    char *m_buffer = new char[m_size];
    for (int i = 0; i < m_size; ++i) {
        m_buffer[i] = m_buf[i];
    }
    int status = write(id, m_buffer, m_size);
    if (status == m_size) {
        return 0;
    } else {
        return -1;
    }

}
///////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
JNIEXPORT jint

JNICALL
Java_com_example_phamh_factoryodroid_RS485_releaseRS485(
        JNIEnv *env,
        jobject parent,
        jint id) {
    int status = close(id);
    return 0;
}