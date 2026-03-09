import serial
import json
import time

PORT = "/dev/ttyUSB0"
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)

i = 0
while True:
    packet = {
        "data0": i,
        "data1": 100 + i,
        "data2": (-10 + i % 20)
    }
    ser.write((json.dumps(packet) + "\n").encode("utf-8"))
    print(packet)
    i += 1
    time.sleep(1)
