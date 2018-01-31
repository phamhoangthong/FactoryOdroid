//
// Created by phamh on 1/30/2018.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "MTCP.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
///////////////////////////////////////////////////////////////////////////////////////////////////
static char genRandom()
{
    return alphanum[rand() % (sizeof(alphanum)-1)];
}
///////////////////////////////////////////////////////////////////////////////////////////////////
static string genRandomStr(int length) {
    string m_str;
    for(unsigned int i = 0; i < length; i++) {
        m_str += genRandom();
    }
    return m_str;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void* MTCP_THREAD(void *param) {
    MTCP *p_tcp = (MTCP*)param;
    p_tcp->run();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
MTCP::MTCP() {
    m_clients = vector<MyClient>();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
MTCP::~MTCP() {
    m_clients.clear();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
MTCP::MyClient::MyClient(int socket, string code, MTCP* &parent) {
    m_parent = parent;
    init(socket,code);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MTCP::MyClient::haveData(string code, char *data, size_t length) {
    if(m_parent != nullptr)
        m_parent->haveDataFromClient(code, data, length);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MTCP::MyClient::haveError(string code, string msg) {
    if(m_parent != nullptr)
        m_parent->haveErrorFromClient(code, msg);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MTCP::MyClient::sended(string code) {
    if(m_parent != nullptr)
        m_parent->sendedDataToClient(code);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MTCP::MyClient::disconnected(string code) {
    if(m_parent != nullptr) {
        m_parent->disconnectedClient(code);
        if(m_parent->disconnectClient(code) != 0) {
            string msg = "Server error - disconnect client [" + code + "] - not finded clients";
            m_parent->haveErrorServer(msg);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MTCP::init(int port) {
    m_port = port;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int MTCP::connectSocket() {
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket < 0) {
        string msg = "Server error opening socket";
        haveErrorServer(msg);
        return -1;
    }
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(m_port);
    if(bind(m_socket,(struct sockaddr *)&server_address,sizeof(server_address)) < 0) {
        string msg = "Server error on binding";
        haveErrorServer(msg);
        return -2;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int MTCP::start() {
    if(connectSocket() != 0) {
        return -1;
    }
    if(flag_run) {
        string msg = "Server error - server running";
        haveErrorServer(msg);
        return -2;
    }
    flag_run = true;
    int rc = pthread_create(&m_thread, NULL, MTCP_THREAD, this);
    if(rc) {
        string msg = "Server error begin";
        haveErrorServer(msg);
        return -3;
    }
    string msg = "Server starting";
    haveStatusServer(msg);
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void MTCP::run() {
    listen(m_socket,10);
    socklen_t m_client_socket_length;
    m_client_socket_length = sizeof(sockaddr_in);
    while(flag_run) {
        int m_new_socket;
        struct sockaddr_in m_client_address;
        m_new_socket = accept(m_socket,(struct sockaddr *)(&m_client_address), &m_client_socket_length);
        if(m_new_socket > 0) {
            // add client
            string m_code;
            while(1) {
                m_code = genRandomStr(LENGTH_CODE);
                bool flag_code = true;
                for(int i = 0; i < m_clients.size(); i++) {
                    if(m_code.compare(m_clients.at(i).getCode()) == 0) {
                        flag_code = false;
                        break;
                    }
                }
                if(flag_code) {
                    break;
                }
            }
            MTCP* m_master = this;
            m_clients.push_back(MyClient(m_new_socket,m_code,m_master));
            string msg = "Server add new client [" + m_code + "]";
            haveStatusServer(msg);
        } else {
            // error accept
            string msg = "Server error accept client";
            haveErrorServer(msg);
        }
        // Thong bao so luong clients
        char * m_msg;
        sprintf(m_msg,"Server have %d clients", m_clients.size());
        string msg = string(m_msg);
        haveStatusServer(msg);
        delete[] m_msg;
        usleep(1);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int MTCP::indexClient(string code) {
    int m_index = 0;
    int m_size = m_clients.size();
    bool m_flag = false;
    for(m_index = 0; m_index < m_size; m_index++) {
        if(code.compare(m_clients.at(m_index).getCode()) == 0) {
            m_flag = true;
            break;
        }
    }
    if(m_flag)
        return m_index;
    else
        return -1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int MTCP::disconnectClient(string code) {
    int m_index = indexClient(code);
    if(m_index < 0) {
        return -1;
    } else {
        m_clients.erase(m_clients.begin() + m_index);
        return 0;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////