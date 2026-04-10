#pragma once
#include <winsock2.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class UdpSender{
public:
    UdpSender(const std::string& host, int port);
    ~UdpSender();

    bool send(const void* data, int length);
private:
    SOCKET sock;
    sockaddr_in dest;
};