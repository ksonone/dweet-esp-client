/*
    Dweet ESP Client
    Author : Kapil H. Sonone
*/

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

const char* host = "dweet.io";
String THING_NAME = "Enter_Thing_Name"; //Enter Thing Name

const char* ssid = "SSID";            //Replace with SSID
const char* password = "PASSWORD";    //Replace with PASSWORD

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(2000);

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/get/latest/dweet/for/" + THING_NAME;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {

    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonBuffer jsonBuffer(capacity);

    String line = client.readString();
    String payload_without_header = line.substring(160);  //Header Size Removed
//    Serial.print(payload_without_header);
//    Serial.println();
    
    JsonObject& payload = jsonBuffer.parseObject(payload_without_header);
    
    if (!payload.success()) {
      Serial.println("parseObject() failed");
      return;
    }

    String payload_with_thing = payload["with"][0]["thing"];
    String thing_1 = payload["with"][0]["content"]["Thing_1"];
    String thing_2 = payload["with"][0]["content"]["Thing_2"];
    String thing_3 = payload["with"][0]["content"]["Thing_3"];
    String thing_4 = payload["with"][0]["content"]["Thing_4"];
    String thing_5 = payload["with"][0]["content"]["Thing_5"];
    
    Serial.print("Thing Name : ");
    Serial.print(payload_with_thing);
    Serial.print(" | Thing 1 : ");
    Serial.print(thing_1);
    Serial.print(" | Thing 2 : ");
    Serial.print(thing_2);
    Serial.print(" | Thing 3 : ");
    Serial.print(thing_3);
    Serial.print(" | Thing 4 : ");
    Serial.print(thing_4);
    Serial.print(" | Thing 5 : ");
    Serial.print(thing_5);
  }

  Serial.println();
  Serial.println("closing connection");
}
