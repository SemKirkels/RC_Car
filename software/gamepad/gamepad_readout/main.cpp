#include <windows.h>
#include <Xinput.h>
#include <iostream>

#pragma comment(lib, "Xinput.lib")

#define POLL_RATE_HZ 1
#define POLL_DELAY_MS (1000/POLL_RATE_HZ)

int main(){
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    for(;;){
        DWORD result = XInputGetState(0, &state);

        if(result == ERROR_SUCCESS){
            std::cout << "Buttons: " << state.Gamepad.wButtons << std::endl;

            std::cout << "Left Stick X: " << state.Gamepad.sThumbLX << std::endl;
            std::cout << "Left Stick Y: " << state.Gamepad.sThumbLY << std::endl;

            std::cout << "Right Stick X: " << state.Gamepad.sThumbRX << std::endl;
            std::cout << "Right Stick Y: " << state.Gamepad.sThumbRY << std::endl;

            std::cout << "Left Trigger: " << (int)state.Gamepad.bLeftTrigger << std::endl;
            std::cout << "Right Trigger: " << (int)state.Gamepad.bRightTrigger << std::endl;

            std::cout << "----------------------" << std::endl;
        } 
        else{
            std::cout << "No controller connected" << std::endl;
        }

        Sleep(POLL_DELAY_MS);
    }

    return 0;
}