#include "fabgl.h"
#include "bitmapsWarehouse.h"
#include "fabui.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "keys/openweathermap_key.h"

#include "ParkingApp.h"
//#include <HardwareSerial.h>
//HardwareSerial mySerial(1);

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
extern const char* ssid = "ICST";
extern const char* password ="arduino123" ;


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
    mySerial.begin(115200, SERIAL_8N1, 34, 2);
  }
  else
  {
    Serial.println("\nFailed to connect to the WiFi network within 10 seconds");

  }

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
      WiFi.disconnect(true, true);
      Serial.println("WiFi Disconnected");
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
      Serial.println("Disconnecting from WiFi...");
      WiFi.disconnect(true, true);
    }

    // ParkingApp().runAsync(&DisplayController, 3500).joinAsyncRun();
    if(weather_succeeded)
      ParkingApp(&myObject).runAsync(&DisplayController, 3500).joinAsyncRun();
    else
      ParkingApp(nullptr).runAsync(&DisplayController, 3500).joinAsyncRun();
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




