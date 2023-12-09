#if defined(ESP8266)
 #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
 #include <WiFi.h>          //https://github.com/esp8266/Arduino
#endif

#include <PubSubClient.h>
#include <ArduinoJson.h>
// Kommunikation zum Luft-Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Luft Definitionen 
Adafruit_BME280 bme;
// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_I2C_ADDRESS  0x76
float temp = 0.0;
float humi = 0.0;
float pres = 0.0;
bool bmeError = false;

/* WLAN */
#define WIFI_SSID "SchuppenMustin"
#define WIFI_PASSWORD "11ff22ff33ff44ff55ff66ff77"

/* MQTT */
const char* MQTT_BROKER = "10.8.196.116";
const char* MQTT_PORT = "1883";
const char* mqttUser = "mqtt";
const char* mqttPassword = "mqtt12";
String myIP;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200); 
  // put your setup code here, to run once:
  connectToWifi();
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);

  Wire.begin(D3,D4);
  if( bme.begin(BME280_I2C_ADDRESS) == 0 ) {
    // connection error or device address wrong!
    Serial.println("bme280 Fehler !!");
    bmeError = true;
  }
}

void loop() {
 if (!client.connected()) {
    while (!client.connected()) {
      client.connect("ESP8266Client", mqttUser, mqttPassword);
      // client.subscribe("test/test/baum");
      delay(100);
    }
 }
   client.loop();
   if ((millis() - lastTime) > timerDelay) {
    readLuftWerte();

    StaticJsonDocument<JSON_OBJECT_SIZE(4)> doc;
    doc["device"] = "Wohnzimmer";
    doc["temperatur"] = (String)temp;
    doc["feuchtigkeit"] = (String)humi;
    doc["lufdruck"] = (String)pres;
    // Declare a buffer to hold the result
    char output[128];
    serializeJson(doc, output);

    Serial.println("Sending message to MQTT topic..");
    Serial.println(output);
    if (client.publish("/skl/", output) == true) {
       Serial.println("Success sending message");
    } else {
      Serial.println("Error sending message");
    }
     lastTime=millis();
   }
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to WiFi network with IP Address: ");
  myIP =  WiFi.localIP().toString().c_str();
  Serial.println(myIP);
}

void callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (byte i = 0; i < length; i++) {
        char tmp = char(payload[i]);
        msg += tmp;
    }
    Serial.println(msg);
}

void readLuftWerte() {
  temp = bme.readTemperature();  // get temperature in °C
  humi = bme.readHumidity();     // get humidity in %
  pres = bme.readPressure()/100;     // get pressure in Pa
}
