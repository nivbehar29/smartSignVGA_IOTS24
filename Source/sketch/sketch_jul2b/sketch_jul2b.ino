#include "fabgl.h"
#include "fabui.h"

#include "WifiMngr.h"
#include "FirebaseMngr.h"
#include <HTTPClient.h>
#include <Arduino_JSON.h>
// #include "pngle.h"
#include "keys/openweathermap_key.h"

#include "ParkingApp.h"
#include "DBAux.h"
#include "Weather.h"

// Main VGA Controller
fabgl::VGA16Controller* DisplayController;
fabgl::PS2Controller   PS2Controller;

// Parking Lot Database
extern DB_parkingLot* db_parkingLot;
bool first_app_run = true;

// Weather
extern JSONVar myObject;
bool done_with_weather = false;
bool weather_succeeded = false;

int quitCounter = 0;

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

void loop() {

  if(!done_with_weather)
  {
    weather_succeeded = getWeatherDB();
    done_with_weather = true;
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
      ParkingApp(&myObject).runAsync(DisplayController, 6000).joinAsyncRun();
    else
      ParkingApp(nullptr).runAsync(DisplayController, 6000).joinAsyncRun();

    // Increment quit counter
    quitCounter++;

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

    if(quitCounter == 5)
    {
      Serial.println("RESTART !!");
      ESP.restart();
    }
  }
}




