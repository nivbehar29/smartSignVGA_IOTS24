#pragma once

#include <WiFi.h>
#include "keys/openweathermap_key.h"
// #include "pngle.h"

// Weather parameters
String city = "Haifa";
String countryCode = "IL";

String jsonBuffer;
JSONVar myObject;

const char* imageUrl = "http://openweathermap.org/img/wn/01n@2x.png";
uint8_t* pngBuffer = nullptr;
size_t pngBufferSize = 0;

#define IMAGE_WIDTH 100
#define IMAGE_HEIGHT 100
//uint8_t weatherIconBuffer[IMAGE_WIDTH * IMAGE_HEIGHT];
uint8_t* weatherIconBuffer = nullptr;

// void pngle_on_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
// {
//   if (x < IMAGE_WIDTH && y < IMAGE_HEIGHT)
//   {
//     int index = y * IMAGE_WIDTH + x;

//     uint8_t r = (rgba[0] >> 6) & 0b11;
//     uint8_t g = (rgba[1] >> 6) & 0b11;
//     uint8_t b = (rgba[2] >> 6) & 0b11;
//     uint8_t a = (rgba[3] >> 6) & 0b11;

//     if(a == 0b00)
//     {
//       r = 0;
//       g = 3;
//       b = 0;
//       a = 3;
//     }

//     weatherIconBuffer[index] = (a << 6) | (b << 4) | (g << 2) | r;
//   }
// }

// void load_png(const char *url, double scale = 1.0)
// {
//   HTTPClient http;

//   http.begin(url);

//   int httpCode = http.GET();
//   if (httpCode != HTTP_CODE_OK) {
//     Serial.printf("HTTP ERROR: %u\n", httpCode);
//     http.end();
//     return ;
//   }

//   weatherIconBuffer = (uint8_t*) malloc(sizeof(uint8_t) * IMAGE_WIDTH * IMAGE_HEIGHT);

//   int total = http.getSize();

//   WiFiClient *stream = http.getStreamPtr();

//   pngle_t *pngle = pngle_new();
//   pngle_set_draw_callback(pngle, pngle_on_draw);
//   double g_scale = scale;

//   uint8_t buf[2048];
//   int remain = 0;
//   uint32_t timeout = millis();
//   while (http.connected()  && (total > 0 || remain > 0)) {

//     // Break out of loop after 10s
//     if ((millis() - timeout) > 10000UL)
//     {
//       Serial.println("HTTP request timeout\n");
//       break;
//     }

//     size_t size = stream->available();

//     if (!size) { delay(1); continue; }

//     if (size > sizeof(buf) - remain) {
//       size = sizeof(buf) - remain;
//     }

//     int len = stream->readBytes(buf + remain, size);
//     if (len > 0) {
//       int fed = pngle_feed(pngle, buf, remain + len);
//       if (fed < 0) {
//         Serial.printf("ERROR: %s\n", pngle_error(pngle));
//         break;
//       }
//       total -= len;
//       remain = remain + len - fed;
//       if (remain > 0) memmove(buf, buf + fed, remain);
//     } else {
//       delay(1);
//     }
//   }

//   pngle_destroy(pngle);

//   http.end();
// }

bool getWeatherDB()
{
  Serial.println("Starting Weather");

  // Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED)
  {
    String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
    
    jsonBuffer = httpGETRequest(serverPath.c_str());
    Serial.println(jsonBuffer);
    myObject = JSON.parse(jsonBuffer);

    // JSON.typeof(jsonVar) can be used to get the type of the var
    // if 'cod' is not 200, according to weather api, we didnt get the right data (can be becuase of bad api key for example)
    if (JSON.typeof(myObject) == "undefined" || (int) myObject["cod"] != 200)
    {
      Serial.println("Parsing input failed!");
      return false;
    }
  
    else
    {
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);

      // String img_url = "http://openweathermap.org/img/wn/10d@2x.png";

      // Serial.println("mem before downloading image:");
      // printMem();

      // load_png(img_url.c_str());

      // Serial.println("mem after downloading image:");
      // printMem();
      


      // Free the PNG buffer after decoding
      // free(pngBuffer);
      // pngBuffer = nullptr;

      return true;

    }
  }
  else
  {
    Serial.println("Wifi is not connected, Abort weather processing");
    return false;
  }
}