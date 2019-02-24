// NEXTION: Constructors

// WiFi: Config
const char* WIFI_SSID = "XXXX";
const char* WIFI_PASSWORD = "XXXX";

// MQTT: Config
const PROGMEM char* MQTT_CLIENT_ID = "NEXTION";
const PROGMEM char* MQTT_SERVER_IP = "192.168.0.100";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "mqttuser";
const PROGMEM char* MQTT_PASSWORD = "mqttpwd";

// MQTT: Topics
const char* MQTT_OUT_TOPIC = "NEXTION/OUT";
const char* MQTT_IN_TOPIC = "NEXTION/IN";

// MQTT: Buffers
char buff[50] = {0};
const String okString = "OK";
bool sent = true;
char msg[50] = {0};
char top[50] = {0};

// PIN: Definitions
//const short unsigned int ledBuilt = 222;
