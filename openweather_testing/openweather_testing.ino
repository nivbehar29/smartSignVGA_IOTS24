#include "fabgl.h"
#include "fabui.h"

#include "WifiMngr.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "keys/openweathermap_key.h"

#include "ParkingApp.h"

// main VGA Controller
fabgl::VGA16Controller* DisplayController;
fabgl::PS2Controller   PS2Controller;

// WIFI
extern String openWeatherMapApiKey;

// Weather parameters
String city = "Haifa";
String countryCode = "IL";
unsigned long lastTime = 0;

// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;

void printMem()
{
  Serial.printf("Free 8bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_8BIT) / 1024);
  Serial.printf("Free 32bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_32BIT) / 1024);
}

void setup(){
  Serial.begin(115200);  // Start the serial communication

  // Wifi
  setupWifi();

}

bool done_with_weather = false;
bool weather_succeeded = false;
JSONVar myObject;

void loop() {

  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay && !done_with_weather) {
    Serial.println("Starting Weather");

    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED)
    {
      Serial.println("Wifi status OK");
      
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      // if 'cod' is not 200, according to weather api, we didnt get the right data (can be becuase of bad api key for example)
      if (JSON.typeof(myObject) == "undefined" || (int) myObject["cod"] != 200)
      {
        Serial.println("Parsing input failed!");
        done_with_weather = true;
        weather_succeeded = false;
      }
    
      else
      {
        done_with_weather = true;
        weather_succeeded = true;

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
      }
    }
    else
    {
      disconnectWifi();
      done_with_weather = true;
      weather_succeeded = false;
    }
    lastTime = millis();
  }

  if(done_with_weather)
  {
    Serial.println("Done with weather!");

    // Disconnect from WiFi
    if(WiFi.status()== WL_CONNECTED)
    {
      disconnectWifi();
    }

    // Setup Mouse / Keyboard
    PS2Controller.begin(PS2Preset::KeyboardPort0_MousePort1, KbdMode::GenerateVirtualKeys);

    DisplayController = new fabgl::VGA16Controller();
    DisplayController->begin();
    DisplayController->setResolution(VGA_640x480_60Hz);

    // Kick off application and wait for it to quit
    if(weather_succeeded)
      ParkingApp(&myObject).runAsync(DisplayController, 3500).joinAsyncRun();
    else
      ParkingApp(nullptr).runAsync(DisplayController, 3500).joinAsyncRun();

    Serial.println("Memory after quit app:");
    printMem();
  }
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




