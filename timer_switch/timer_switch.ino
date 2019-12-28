#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

char* ssid = "wifiname";
char* password = "wifipass";
const char* host = "hostname.xxx";

int count1 = 0;
int count2 = 0;
const int relay = D2;
int api_switch1 = 0;


void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay, OUTPUT);


  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    Serial.print("" + (String)count1 + " - ");
    Serial.println(ssid);
    count1 ++;

    
    WiFi.begin(ssid, password);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);

  } //end while

  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);

}


//=========================
String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}


void loop() {
  

  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);


  String url = "/api/receive/arduino_timer_switch?c=" + 
                "&c=" + (String)count2 +
                "&ip=" + (String)  ipToString(WiFi.localIP()) +
                "&ssid=" +  (String) ssid +
                "&rssi=" +  (String) WiFi.RSSI() +
                "&mac=" +  (String) WiFi.macAddress()+
               

// Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(host + url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      // TODO: Parsing
      Serial.println(payload);


 const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 
      2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 
      JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 390;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.parseObject(payload);
  
if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return ;
  }

  api_switch1 = root["switch1"];


  if( api_switch1 == 1){
      digitalWrite(relay1, HIGH);
      Serial.println("relay1 ON");
    }else{
      digitalWrite(relay1, LOW);
      Serial.println("relay1 OFF");
    }

    } // end (httpCode > 0)
    
    http.end();
    count2++;
    
    } // end (WiFi.status() == WL_CONNECTED)
    

 delay(500);
}
