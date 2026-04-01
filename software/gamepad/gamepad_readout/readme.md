# Controller readout 
The Xbox gamepad can be read out using a Microsoft C/C++ library called __XInput__. This library parses the controller input into several fields, representing both digital and analog inputs.
The controller state is stored in a structure called:  
```cpp
XINPUT_STATE state;
```
## Library output data
The controller data is split into 7 values:
- 1 digital (bitmask) value
- 6 analog values

### Output fields
```cpp
state.Gamepad.wButtons  

state.Gamepad.sThumbLX  
state.Gamepad.sThumbLY  

state.Gamepad.sThumbRX  
state.Gamepad.sThumbRY  

state.Gamepad.bLeftTrigger  
state.Gamepad.bRightTrigger 
```

## Analog input ranges
| Input             | Range             |
|-------------------|-------------------|
| Left stick [X/Y]  | [-32768 ; +32767] |
| Right stick [X/Y] | [-32768 ; +32767] |
| Triggers [LT/RT]  | [0 ; 255]         |

## Digital inputs
Digital inputs are stored in a single variable:
```cpp
state.Gamepad.wButtons
```
Each button corresponds to a bitmask value.
You can check if a button is pressed using a bitwise AND (&).
For example:
```cpp
if(state.Gamepad.wButtons & 0x0001){
    // D-Pad Up pressed
}
```

## Button values
| Controller section | Input     |Value (Hex) |
|--------------------|-----------|------------|
| D-Pad              |           |            |
|                    | Up        | 0x0001     |
|                    | Down      | 0x0002     |
|                    | Left      | 0x0004     |
|                    | Right     | 0x0008     |
| Extra buttons      |           |            |
|                    | Start     | 0x0010     |
|                    | Back      | 0x0020     |
| Joysticks          |           |            |
|                    | Left BTN  | 0x0040     |
|                    | Right BTN | 0x0080     |
| Rear buttons       |           |            |
|                    | LB        | 0x0100     |
|                    | RB        | 0x0200     |
| Buttons            |           |            |
|                    | A         | 0x1000     |
|                    | B         | 0x2000     |
|                    | X         | 0x4000     |
|                    | Y         | 0x8000     |
