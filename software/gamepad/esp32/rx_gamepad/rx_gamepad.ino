#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#define SSID      ""
#define PASSWD    ""
#define UDP_PORT  40450
#define BUFF_SIZE 64

#pragma pack(push, 1)
struct GamepadState{
    bool            connected     = false;
    unsigned short  buttons       = 0;
    unsigned char   leftTrigger   = 0;
    unsigned char   rightTrigger  = 0;
    short           thumbLX       = 0;
    short           thumbLY       = 0;
    short           thumbRX       = 0;
    short           thumbRY       = 0;
};
#pragma pack(pop)

WiFiUDP          udp;
WebServer        server(80);
WebSocketsServer webSocket(81);
GamepadState gs{};
bool started = false;
String lastTof  = "";

const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title>RC Car ToF</title>
        <style>
            body{
                font-family: monospace;
                background: #111;
                color: #00ff88;
                display: flex;
                flex-direction: column;
                align-items: center;
                padding: 40px;
            }

            h1{ 
              color: white; 
              margin-bottom: 30px;
            }

            .grid{
                display: grid;
                grid-template-columns: repeat(3, 1fr);
                gap: 20px;
                width: 100%;
                max-width: 900px;
            }

            .card{
                background: #111;
                border: 1px solid #00ff88;
                border-radius: 12px;
                padding: 30px;
                text-align: center;
            }

            .label{ 
                color: white; 
                font-size: 14px; 
            }

            .value{ 
                font-size: 48px; margin: 10px 0; 
            }

            .unit{ 
                color: white; font-size: 14px; 
            }

            .status{
                position: fixed;
                top: 10px;
                right: 10px;
                padding: 5px 10px;
                border-radius: 4px;
            }

            .connected{
                background: #004400; 
                color: #00ff88;
            }

            .disconnected{ 
                background: #440000; 
                color: #ff0000;
            }

        </style>
    </head>
    <body>
        <div class="status disconnected" id="status">
            Connecting...
        </div>
        <h1>ToF Sensoren</h1>

        <div class="grid">
            <div class="card">
                <div class="label">Left</div>
                <div class="value" id="tof_left">--</div>
                <div class="unit">mm</div>
            </div>
            <div class="card">
                <div class="label">Front</div>
                <div class="value" id="tof_front">--</div>
                <div class="unit">mm</div>
            </div>
            <div class="card">
                <div class="label">Right</div>
                <div class="value" id="tof_right">--</div>
                <div class="unit">mm</div>
            </div>
        </div>

        <script>
            const ws = new WebSocket('ws://' + location.hostname + ':81/');

            ws.onopen = () => {
                document.getElementById('status').className   = 'status connected';
                document.getElementById('status').textContent = 'Connected';
            };

            ws.onclose = () => {
                document.getElementById('status').className   = 'status disconnected';
                document.getElementById('status').textContent = 'Disconnected';
            };

            ws.onmessage = (event) => {
                const tof = event.data.replace('T:', '').split(',');
                tof.forEach(t => {
                    const kv = t.split(':');
                    if(kv[0] === 'F') document.getElementById('tof_front').textContent = kv[1];
                    if(kv[0] === 'L') document.getElementById('tof_left').textContent  = kv[1];
                    if(kv[0] === 'R') document.getElementById('tof_right').textContent = kv[1];
                });
            };
        </script>
    </body>
</html>
)rawliteral";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length){
  if(type == WStype_CONNECTED && lastTof.length() > 0){
    webSocket.sendTXT(num, lastTof);
  }
}

void setup(){
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(SSID, PASSWD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }

  Serial.println(WiFi.localIP());
  udp.begin(UDP_PORT);

      server.on("/", [](){
        server.send(200, "text/html", HTML_PAGE);
    });

    server.begin();
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop(){
  /* Wait for start condition */
  while(!started){
    if(Serial.read() == 'R'){
      started = true;
    }
  }

  server.handleClient();
  webSocket.loop();
  
  char buf[BUFF_SIZE];
  int size = udp.parsePacket();

  if(size == sizeof(GamepadState)){
    udp.read((char*)&gs, sizeof(GamepadState));

    if(!gs.connected){
      /* Send all zeroes, stop car */
      gs.buttons = 0;
      gs.leftTrigger = 0;
      gs.rightTrigger = 0;
      gs.thumbLX = 0;
      gs.thumbLY = 0;
      gs.thumbRX = 0;
      gs.thumbRY = 0;
    }
    
    /* Format: connected > buttons > triggers > sticks < */
    snprintf(buf, sizeof(buf), ">%d>%u>%u>%u>%d>%d>%d>%d<",gs.connected, gs.buttons, gs.leftTrigger, gs.rightTrigger, gs.thumbLX, gs.thumbLY, gs.thumbRX, gs.thumbRY);
    Serial.println(buf);
  }

  if(Serial.available()){
    String line = Serial.readStringUntil('\n');
    if(line.startsWith("T:")){
      lastTof = line;
      webSocket.broadcastTXT(line);
    }
  }
}