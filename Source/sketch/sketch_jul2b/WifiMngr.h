#pragma once

#include <WiFi.h>
#include "keys/keys.h"

bool setupWifi()
{
  WiFi.mode(WIFI_STA);
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