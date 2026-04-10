#include <WiFi.h>
#include <WiFiUdp.h>

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

WiFiUDP udp;
GamepadState gs{};

void setup(){
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(SSID, PASSWD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }

  Serial.println(WiFi.localIP());
  udp.begin(UDP_PORT);
}

void loop(){
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
    snprintf(buf, sizeof(buf), "%d>%u>%u>%u>%d>%d>%d>%d<",gs.connected, gs.buttons, gs.leftTrigger, gs.rightTrigger, gs.thumbLX, gs.thumbLY, gs.thumbRX, gs.thumbRY);
    Serial.println(buf);
  }
}
