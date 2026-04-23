Only works on MS Windows.  
Compile with:
```
g++ *.cpp -o tx_gamepad.exe -lXinput9_1_0 -lws2_32
```
Or for a portable verion (No compiler required on client pc)
```
g++ *.cpp -o tx_gamepad.exe -static -lXinput9_1_0 -lws2_32
```
