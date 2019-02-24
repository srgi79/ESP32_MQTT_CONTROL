#if defined(ESP8266)
  #include <ESP8266WiFi.h>        
#else //def ESP32
  #include <WiFi.h>
#endif
#include <PubSubClient.h>
#include "configuration.h"
#include "Nextion.h"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

NexPage page0    = NexPage(0, 0, "page0");

// Declare a text object [page id:0,component id:1, component name: "t100"].
NexText textNumber = NexText(0, 1, "t100");
// Declare a text object [page id:0,component id:2, component name: "t101"].
NexText textNumber2 = NexText(0, 2, "t101");
// Declare a button object [page id:0,component id:3, component name: "SEND"].
NexButton btnSend = NexButton(0, 3, "SEND");
// Declare a text object [page id:0,component id:4, component name: "t0"].
NexText textNumber4 = NexText(0, 4, "t0");
// Declare a text object [page id:0,component id:5, component name: "t1"].
NexText textNumber5 = NexText(0, 5, "t1");

// Register object textNumber, btnPlus, btnMinus, to the touch event list.
NexTouch *nex_listen_list[] =
{
  &textNumber,
  &textNumber2,
  &textNumber4,
  &textNumber5,
  &btnSend,
  NULL
};

void btnSendCallback(void *ptr)
{
  memset(top, 0, sizeof(top)); // clear buff
  memset(msg, 0, sizeof(msg)); // clear buff
  dbSerialPrintln("btnSendCallback");
  textNumber4.getText(top, sizeof(top));
  textNumber5.getText(msg, sizeof(msg));
  memset(buff, 0, sizeof(buff)); // clear buff
  okString.toCharArray(buff, sizeof(buff));
  textNumber4.setText(buff);
  textNumber5.setText(buff);
  sent = false;
}

void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
 // Byte to String
 String payload;
 int LDR = 0;
 for (uint8_t i = 0; i < p_length; i++) {
   payload.concat((char)p_payload[i]); }
  Serial.print("Rebut: "); Serial.println(payload);
  if (payload == "LDR") { 
    //
    client.publish(MQTT_OUT_TOPIC, String(LDR).c_str());
    client.subscribe(MQTT_IN_TOPIC); }
}

void setup(void)
{
  // Set the baudrate which is for debug and communicate with Nextion screen
  nexInit();
  Serial.begin(115200);
  // Register the pop event callback function of the current button0 component.
  btnSend.attachPop(btnSendCallback);
  dbSerialPrintln("Setup done");

  // init WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connectant a ");
  WiFi.mode(WIFI_STA);
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print("."); }
  
  Serial.println("");
  Serial.println("WiFi OK");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  
  // init MQTT
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback);
}

void reconnect() {
  // Loop fins connexio
  while (!client.connected()) {
   Serial.print("Intentant MQTT...");
   if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
     Serial.println("OK");
     client.subscribe(MQTT_IN_TOPIC); } 
   else {
     Serial.print("ERROR = ");
     Serial.print(client.state());
     Serial.println("Reintentant en 5s");
     delay(5000); }
  }
}

void loop() { // run over and over
  nexLoop(nex_listen_list);
  if (!client.connected()) { reconnect(); }
  client.loop();
  if (not sent) {
    sent = true;
    if (client.publish(top, msg)) {
      Serial.print("MQTT ["); Serial.print(top); 
      Serial.print(","); Serial.print(msg); Serial.println("]");
    }
    else {
      Serial.println("Publish failed");
    }
  }
}
