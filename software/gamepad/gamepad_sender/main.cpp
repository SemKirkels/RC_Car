#include <iostream>
#include "udp_sender.h"
#include "gamepad.h"

#pragma comment(lib, "Xinput.lib")

/* Defines */
#define DEBUG FALSE
#define RECONNECT_DELAY 3000

/* Redout defines */
#define POLL_RATE_HZ 10
#define POLL_DELAY_MS (1000/POLL_RATE_HZ)

/* Network defines */
#define UDP_HOST        "192.168.50.65"
#define UDP_PORT        40450

int main(){
    bool flag_connected = FALSE;
    std::cout << "Starting up" << std::endl;
    std::cout << "Target: " << UDP_HOST << ":" << UDP_PORT << std::endl;

    UdpSender udp(UDP_HOST, UDP_PORT);

    for(;;){
        GamepadState gs = pollGamepad(0);

        if(gs.connected){
            if(flag_connected == FALSE){
                std::cout << "Controller connected" << std::endl;
                flag_connected = TRUE;
            }

            udp.send(&gs, sizeof(gs));

            if(DEBUG){
                std::cout << "Buttons: "  << gs.buttons           << std::endl;
                std::cout << "LT: "       << (int)gs.leftTrigger  << std::endl;
                std::cout << "RT: "       << (int)gs.rightTrigger << std::endl;
                std::cout << "LX: "       << gs.thumbLX           << std::endl;
                std::cout << "LY: "       << gs.thumbLY           << std::endl;
                std::cout << "RX: "       << gs.thumbRX           << std::endl;
                std::cout << "RY: "       << gs.thumbRY           << std::endl;
            }
        }
        else{
            flag_connected = FALSE;
            std::cout << "No controller connected, retry in: " << RECONNECT_DELAY/1000 << "s" << std::endl;
            Sleep(RECONNECT_DELAY);
        }
        Sleep(POLL_DELAY_MS);
    }
    return 0;
}