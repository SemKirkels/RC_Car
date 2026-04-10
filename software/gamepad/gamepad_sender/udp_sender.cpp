#include "udp_sender.h"
#include <ws2tcpip.h>
#include <stdexcept>

UdpSender::UdpSender(const std::string& host, int port){
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0){
        throw std::runtime_error("WSAStartup failed");
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == INVALID_SOCKET){
        throw std::runtime_error("socket() failed");
    }

    dest = {};
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &dest.sin_addr);
}

UdpSender::~UdpSender(){
    closesocket(sock);
    WSACleanup();
}

bool UdpSender::send(const void* data, int length){
    bool status = sendto(   sock, 
                            (const char*)data, 
                            length, 
                            0, 
                            (sockaddr*)&dest, 
                            sizeof(dest));
    return status != SOCKET_ERROR;            
}
