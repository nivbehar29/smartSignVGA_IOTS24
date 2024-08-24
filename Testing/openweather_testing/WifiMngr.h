#pragma once

#include <WiFi.h>
#include "keys/openweathermap_key.h"
#include <HTTPClient.h>


bool setupWifi()
{
  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
  {
      Serial.print(".");
      delay(100);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  else
  {
    Serial.println("\nFailed to connect to the WiFi network within 10 seconds");
    WiFi.disconnect(true, true);
    return false;
  }
}

void disconnectWifi()
{
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_OFF);

  Serial.println("WiFi Disconnected");
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}