#include "fabgl.h"
#include "bitmapsWarehouse.h"
#include "fabui.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "keys/openweathermap_key.h"

#include "ParkingApp.h"

// main VGA Controller
fabgl::VGA16Controller DisplayController;
fabgl::PS2Controller   PS2Controller;

// bitmap for parking image
// Bitmap park_bitmap = Bitmap(92, 146, NBitmapsWarehouse::park, PixelFormat::RGBA2222);
// Bitmap red_light_bitmap = Bitmap(60, 52, NBitmapsWarehouse::red_light, PixelFormat::RGBA2222);

// sprites (images) to show on the screen
// Sprite sprites[4];

// main Canvas
// fabgl::Canvas Canvas(&DisplayController);
//fabgl::uiApp runner;

// WIFI
extern String openWeatherMapApiKey;
const char* ssid = "ICST";
const char* password = "arduino123";
// const char* ssid = "Home12";
// const char* password = "Dira1202";


// Weather parameters
String city = "Haifa";
String countryCode = "IL";
// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;



void setup(){
  Serial.begin(115200);  // Start the serial communication

  // Setup Mouse / Keyboard
  PS2Controller.begin(PS2Preset::KeyboardPort0_MousePort1, KbdMode::GenerateVirtualKeys);

  // Setup display controller
  DisplayController.begin();
  DisplayController.setResolution(VGA_640x480_60Hz); // VGA_640x350_70Hz , VGA_640x480_60Hz

  // Wifi
  delay(1000);

  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

}

bool done_with_weather = false;
JSONVar myObject;

void loop() {

  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay && !done_with_weather) {
    Serial.println("Starting Weather");

    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      Serial.println("Wifi status OK");
      
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      else
      {
        done_with_weather = true;
      }
    
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
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }

  if(done_with_weather)
  {
    Serial.println("Done with weather!");

    // Disconnect from WiFi
    Serial.println("Disconnecting from WiFi...");
    WiFi.disconnect(true, true);

    // ParkingApp().runAsync(&DisplayController, 3500).joinAsyncRun();
    ParkingApp(myObject).runAsync(&DisplayController, 3500).joinAsyncRun();
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
