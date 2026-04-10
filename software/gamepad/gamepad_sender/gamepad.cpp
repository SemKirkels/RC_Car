#include "gamepad.h"
#include <cstring>

GamepadState pollGamepad(DWORD controllerIndex){
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    GamepadState result{};
    result.connected = (XInputGetState(controllerIndex, &state) == ERROR_SUCCESS);

    if(result.connected){
        result.buttons      = state.Gamepad.wButtons;
        result.leftTrigger  = state.Gamepad.bLeftTrigger;
        result.rightTrigger = state.Gamepad.bRightTrigger;
        result.thumbLX      = state.Gamepad.sThumbLX;
        result.thumbLY      = state.Gamepad.sThumbLY;
        result.thumbRX      = state.Gamepad.sThumbRX;
        result.thumbRY      = state.Gamepad.sThumbRY;
    }
    return result;
}