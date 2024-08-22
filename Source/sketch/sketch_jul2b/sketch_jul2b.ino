#include "fabgl.h"
#include "fabui.h"

#include "WifiMngr.h"
#include "FirebaseMngr.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "keys/keys.h"

#include "ParkingApp.h"
#include "DBAux.h"

// main VGA Controller
fabgl::VGA16Controller* DisplayController;
fabgl::PS2Controller   PS2Controller;

// Weather parameters
extern String openWeatherMapApiKey; // weather api key which should be placed in keys/keys.h
String city = "Haifa";
String countryCode = "IL";
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
JSONVar weatherObject;              // holds the json data pulled from weather api
String jsonBuffer;                  // holds the json data pulled from weather api (as string)
bool done_with_weather = false;     // Indicates if weather pulling has been done - successfull or not
bool weather_succeeded = false;     // Indicated if weather pulling succeeded

// Database pointer - defined in DBAux.h
extern DB_parkingLot* db_parkingLot;

// App parameters
bool first_app_run = true;
int quitCounter = 0;

// prints memory for debugging
void printMem()
{
  Serial.printf("Free 8bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_8BIT) / 1024);
  Serial.printf("Free 32bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_32BIT) / 1024);
}

void setup(){
  Serial.begin(115200);  // Start the serial communication
  setupWifi();  // Setup Wifi
}

void loop() {

  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay && !done_with_weather) {
    Serial.println("Starting Weather");

    // If wifi is connected, pull weather data
    if(WiFi.status()== WL_CONNECTED)
    {
      Serial.println("Wifi status OK");
      
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());

      // For printing the data, use:
      // Serial.println(jsonBuffer);
      weatherObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      // If 'cod' is not 200, according to weather api, we didn't get the right data (can be becuase of bad api key for example)
      if (JSON.typeof(weatherObject) == "undefined" || (int) weatherObject["cod"] != 200)
      {
        Serial.println("Parsing input failed!");
        weather_succeeded = false;
      }
      else
      {
        weather_succeeded = true;
      }
    }
    else
    {
      disconnectWifi();
      weather_succeeded = false;
    }

    done_with_weather = true;
    lastTime = millis();
  }

  if(done_with_weather)
  {
    Serial.println("Done with weather!");

    // Get parkinglot database frome firebase
    if(first_app_run && WiFi.status()== WL_CONNECTED)
    {
      // Setup Firebase
      FBMngr* fbMngr = new FBMngr();
      fbMngr->setup();

      // Get data from firebase
      bool DB_status = fbMngr->getDB2();
      if(!DB_status)
      {
        Serial.println("Error occured during pulling database");
      }

      // End firebase
      fbMngr->EndFB();
      delete fbMngr;
      
      disconnectWifi();
    }

    // Setup Mouse / Keyboard (2 cases - first run \ not first run)
    if(first_app_run)
    {
      PS2Controller.begin(PS2Preset::KeyboardPort0_MousePort1, KbdMode::GenerateVirtualKeys);
      DisplayController = new fabgl::VGA16Controller();
      DisplayController->begin();
      DisplayController->setResolution(VGA_640x480_60Hz);
    }
    else
    {
      DisplayController = new fabgl::VGA16Controller();
      DisplayController->begin();
      DisplayController->setResolution(VGA_640x480_60Hz);
      PS2Controller.mouse()->reset();
    }

    // Kick off application and wait for it to quit (quits when the user chose a parking slot)
    if(weather_succeeded)
      ParkingApp(&weatherObject).runAsync(DisplayController, 3500).joinAsyncRun();
    else
      ParkingApp(nullptr).runAsync(DisplayController, 3500).joinAsyncRun();

    // Increment the app quit counter
    quitCounter++;

    // Set display to low resolution, to get some memory for wifi + firebase
    DisplayController->setResolution(VGA_256x384_60Hz);

    // Make a 'Thank You' canvas and show it on the screen
    Serial.println("Set canvas...");
    Canvas cv(DisplayController);
    cv.setBrushColor(RGB888(0, 0, 0));
    cv.clear();
    cv.setPenColor(Color::White);

    // 'Thank you' text
    int ThankYouTextLength = cv.textExtent(&fabgl::FONT_std_24, "Thank You! :)");
    cv.drawText(&fabgl::FONT_std_24, 256/2 - ThankYouTextLength/2, 384/2, "Thank You! :)");

    // wait for 3 seconds before turning of the screen
    Serial.println("wait 3 sec");
    delay(3000);

    // This is for handling wifi issues when trying to update the firebase with the new data
    bool is_firebase_updated = false;

    // Set up wifi and update firebase
    setupWifi();
    if(WiFi.status()== WL_CONNECTED)
    {
      FBMngr* fbMngr = new FBMngr();
      fbMngr->setup();

      // send new database to firebase
      is_firebase_updated = fbMngr->setDB();

      fbMngr->EndFB();
      delete fbMngr;
      
      disconnectWifi();
    }

    // End and delete the display controller before making anew one on the next loop
    DisplayController->end();
    delete DisplayController;

    // Indicate that the next app run is not the first
    first_app_run = false;

    // Restart when quitCounter >= 5 (to handle our known bug - cutoff at the button of the screen)
    // But - if we didnt succeed updating firebase (wifi issue) - don't restart
    if(quitCounter >= 5 && is_firebase_updated == true)
    {
      Serial.println("RESTART !!");
      ESP.restart();
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
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




