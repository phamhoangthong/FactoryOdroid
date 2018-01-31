///////////////////////////////////////////////////////////////////////////////////////////////////
#include <jni.h>
#include <string>
#include "MSerialPort.h"
#include "MTCP.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
class MySerialPort : public MSerialPort {
private:
    JNIEnv *m_env;
    jobject m_parent;
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void setInfo(JNIEnv *env,jobject parent) {
        m_env = env;
        m_parent = parent;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void haveData(char* buf, size_t size) {
        jmethodID m_methodID = m_env->GetMethodID(m_env->GetObjectClass(m_parent), "sendDataToHandler", "([C)V");
        jchar * p_buf = new jchar[size];
        for(int i = 0; i < size; i++) {
            p_buf[i] = (jchar)buf[i];
        }
        jcharArray m_buffer = m_env->NewCharArray(size);
        m_env->SetCharArrayRegion(m_buffer,0,10,p_buf);
        m_env->CallVoidMethod(m_parent, m_methodID, m_buffer);
        delete[](p_buf);
        m_env->DeleteLocalRef(m_buffer);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
};
///////////////////////////////////////////////////////////////////////////////////////////////////
class MyTCP : public MTCP {
private:
    JNIEnv *m_env;
    jobject m_parent;
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void setInfo(JNIEnv *env,jobject parent) {
        m_env = env;
        m_parent = parent;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void haveDataFromClient(string code, char* data, size_t length) {

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void haveErrorFromClient(string code, string msg) {

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void sendedDataToClient(string code) {

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void disconnectedClient(string code) {

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void haveStatusServer(string msg) {

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    void haveErrorServer(string msg) {

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
};
///////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
JNIEXPORT jlong

JNICALL
Java_com_example_phamh_factoryodroid_MSerialCommunication_createSerial(
        JNIEnv *env,
        jobject parent, jstring port, jint speed) {
    MySerialPort p_serial_port = MySerialPort();
    const char *portString = env->GetStringUTFChars(port, JNI_FALSE);
    p_serial_port.connect(portString, speed);
    p_serial_port.setInfo(env, parent);
    p_serial_port.start();
    return (jlong)(&p_serial_port);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
JNIEXPORT jint

JNICALL
Java_com_example_phamh_factoryodroid_MSerialCommunication_sendToSerial(
        JNIEnv *env,
        jobject parent,jlong obj, jcharArray data) {
    MySerialPort *p_serial_port = (MySerialPort *)(obj);
    int m_size = env->GetArrayLength(data);
    jchar *m_buf = new jchar[m_size];
    env->GetCharArrayRegion(data,0,m_size,m_buf);
    char* m_buffer = new char[m_size];
    for (int i = 0; i < m_size; ++i) {
        m_buffer[i] = m_buf[i];
    }
    p_serial_port->sendData(m_buffer, m_size);
    delete[](m_buffer);
    delete[](m_buf);
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
JNIEXPORT jlong

JNICALL Java_com_example_phamh_factoryodroid_MTCPCommunication_createServer(
        JNIEnv *env,
        jobject parent,
        jint port) {
    MyTCP myTCP;

}
///////////////////////////////////////////////////////////////////////////////////////////////////