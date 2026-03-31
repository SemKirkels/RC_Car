#include <iostream>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "Ws2_32.lib")
    using SocketType = SOCKET;
    #define INVALID   INVALID_SOCKET
    #define CLOSE(s)  closesocket(s)
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    using SocketType = int;
    #define INVALID   -1
    #define CLOSE(s)  close(s)
#endif

#define UDP_PORT 40450

#pragma pack(push, 1)
struct GamepadState{
    bool  connected;
    unsigned short buttons;
    unsigned char leftTrigger;
    unsigned char rightTrigger;
    short thumbLX;
    short thumbLY;
    short thumbRX;
    short thumbRY;
};
#pragma pack(pop)

int main(){
    
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    SocketType sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == INVALID){
        std::cerr << "socket() failed\n";
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(UDP_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0){
        std::cerr << "bind() failed\n";
        return 1;
    }

    std::cout << "Listening on port " << UDP_PORT << std::endl;

    GamepadState gs{};
    for(;;){
        int received = recv(sock, (char*)&gs, sizeof(gs), 0);

        if(received <= 0){
            continue;
        }

        if(gs.connected){
            std::cout << "Buttons: "  << gs.buttons           << std::endl;
            std::cout << "LT: "       << (int)gs.leftTrigger  << std::endl;
            std::cout << "RT: "       << (int)gs.rightTrigger << std::endl;
            std::cout << "LX: "       << gs.thumbLX           << std::endl;
            std::cout << "LY: "       << gs.thumbLY           << std::endl;
            std::cout << "RX: "       << gs.thumbRX           << std::endl;
            std::cout << "RY: "       << gs.thumbRY           << std::endl;
        } 
        else{
            std::cout << "Controller disconnected\n";
        }
    }
    CLOSE(sock);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
