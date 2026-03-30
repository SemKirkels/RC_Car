#pragma once
#include <windows.h>
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

#pragma pack(push, 1)
struct GamepadState{
    bool connected;
    WORD  buttons;
    BYTE  leftTrigger;
    BYTE  rightTrigger;
    SHORT thumbLX;
    SHORT thumbLY;
    SHORT thumbRX;
    SHORT thumbRY;
};
#pragma pack(pop)

GamepadState pollGamepad(DWORD controllerIndex = 0);