#include "fabgl.h"
#include "fabui.h"

#include "WifiMngr.h"
#include "FirebaseMngr.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "keys/openweathermap_key.h"

#include "ParkingApp.h"
#include "DBAux.h"
//#include <HardwareSerial.h>
//HardwareSerial mySerial(1);

// main VGA Controller
fabgl::VGA16Controller* DisplayController;
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
// extern const char* ssid = "ICST";
// extern const char* password ="arduino123" ;


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

void printMem()
{
  Serial.printf("Free 8bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_8BIT) / 1024);
  Serial.printf("Free 32bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_32BIT) / 1024);
}

void setup(){
  Serial.begin(115200);  // Start the serial communication

  Serial.println("Memory at setup():");
  printMem();

  // Setup display controller
  // DisplayController = new fabgl::VGA16Controller();
  // DisplayController->begin();
  // DisplayController->setResolution(VGA_640x480_60Hz); // VGA_640x350_70Hz , VGA_640x480_60Hz

  Serial.println("Memory before wifi:");
  printMem();
  // Wifi
  setupWifi();

}

bool done_with_weather = true;
bool weather_succeeded = false;
JSONVar myObject;
extern DB_parkingLot* db_parkingLot;
bool first_app_run = true;

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

      printMem();
      
      disconnectWifi();
    }

    // Setup Mouse / Keyboard
    if(first_app_run)
    {
      PS2Controller.begin(PS2Preset::KeyboardPort0_MousePort1, KbdMode::GenerateVirtualKeys);

      DisplayController = new fabgl::VGA16Controller();
      DisplayController->begin();
      DisplayController->setResolution(VGA_640x480_60Hz);
      // delay(5000);
    }
    else
    {
      DisplayController = new fabgl::VGA16Controller();
      DisplayController->begin();
      DisplayController->setResolution(VGA_640x480_60Hz);
      // delay(5000);
      // DisplayController->setResolution(VGA_256x384_60Hz);
      PS2Controller.mouse()->reset();
    }
    

    Serial.println("Memory before init app:");
    printMem();

    // Kick off application and wait for it to quit
    if(weather_succeeded)
      ParkingApp(&myObject).runAsync(DisplayController, 3500).joinAsyncRun();
    else
      ParkingApp(nullptr).runAsync(DisplayController, 3500).joinAsyncRun();

    Serial.println("Memory after quit app:");
    printMem();


    // Set display to low resolution, to get some memory for wifi + firebase
    DisplayController->setResolution(VGA_256x384_60Hz);
    // delete DisplayController;

    Serial.println("Memory after set resolution to VGA_256x384_60Hz:");
    printMem();

    // Serial.println("wait 10 sec");
    // delay(10000);

    Serial.println("Set canvas...");
    Canvas cv(DisplayController);
    cv.setBrushColor(RGB888(255, 255, 255));
    cv.clear();
    cv.setPenColor(Color::Black);

    int ThankYouTextLength = cv.textExtent(&fabgl::FONT_10x20, "Thank You! :)");
    cv.drawText(&fabgl::FONT_10x20, 256/2 - ThankYouTextLength/2, 384/2, "Thank You! :)");

    Serial.println("wait 3 sec");
    delay(3000);

    Serial.println("trying firebase again");
    setupWifi();
    if(WiFi.status()== WL_CONNECTED)
    {
      FBMngr* fbMngr = new FBMngr();
      fbMngr->setup();

      // send new database to firebase
      fbMngr->setDB();

      fbMngr->EndFB();
      delete fbMngr;
      
      disconnectWifi();
    }

    DisplayController->end();
    delete DisplayController;

    first_app_run = false;

    // Serial.println("RESTART !!");
    // ESP.restart();

    // // Kick off application and wait for it to quit
    // if(weather_succeeded)
    //   ParkingApp(&myObject).runAsync(&DisplayController, 3500).joinAsyncRun();
    // else
    //   ParkingApp(nullptr).runAsync(&DisplayController, 3500).joinAsyncRun();
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




