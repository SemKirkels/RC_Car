#include <WiFi.h>
#include <WiFiUdp.h>

#define SSID      ""
#define PASSWD    ""
#define UDP_PORT  40450

#pragma pack(push, 1)
struct GamepadState{
    bool  connected;
    short buttons;
    unsigned char leftTrigger;
    unsigned char rightTrigger;
    short thumbLX;
    short thumbLY;
    short thumbRX;
    short thumbRY;
};
#pragma pack(pop)

WiFiUDP udp;
GamepadState gs{};

void setup(){
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(SSID, PASSWD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\r\nConnected");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  udp.begin(UDP_PORT);
  Serial.printf("Listening on UDP port %d\n", UDP_PORT);
}

void loop(){
  int size = udp.parsePacket();

  if(size == sizeof(GamepadState)){
    udp.read((char*)&gs, sizeof(GamepadState));

    if(gs.connected){
      Serial.printf("Buttons: %d\n", gs.buttons);
      Serial.printf("LT: %d  RT: %d\n", gs.leftTrigger, gs.rightTrigger);
      Serial.printf("LX: %d  LY: %d\n", gs.thumbLX, gs.thumbLY);
      Serial.printf("RX: %d  RY: %d\n", gs.thumbRX, gs.thumbRY);
      Serial.println("----------------------");
    }
    else{
      Serial.println("Controller disconnected");
    }
  }
}
