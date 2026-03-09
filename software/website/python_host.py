from flask import Flask, jsonify
import serial
import json
import threading

# ===== CONFIG =====
UART_PORT = "/dev/ttyAMA0"
UART_BAUD = 115200
WEB_HOST = "192.168.1.6"
WEB_PORT = 5000
INDEX_FILE = "index.html"
# ==================

app = Flask(__name__)

newData = {}

ser = serial.Serial(UART_PORT, UART_BAUD)


def readUart():
    global newData
    while True:
        line = ser.readline().decode().strip()

        try:
            newData = json.loads(line)
        except:
            pass


@app.route("/data")
def data():
    return jsonify(newData)


@app.route("/")
def index():
    return open(INDEX_FILE).read()


thread = threading.Thread(target=readUart)
thread.daemon = True
thread.start()

app.run(host=WEB_HOST, port=WEB_PORT)
