#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#define DHT11PIN 13

const char* ssid = "Hotspot";
const char* password = "password";
const char* serverName = "http://192.168.x.x:5000/sensor/data";


DHT dht(DHT11PIN, DHT11);

void setup()
{
  Serial.begin(115200);
  /* Start the DHT11 Sensor */
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

}

void loop()
{
  int temperature = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("ºC ");
  postData(temperature);

  
  delay(7000);
}


void postData(int temperature) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Create form data
    char httpRequestData[100];
    sprintf(httpRequestData, "temperature=%d", temperature);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}
