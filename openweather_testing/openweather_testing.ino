#include "fabgl.h"
#include "fabui.h"

#include "WifiMngr.h"
#include <Arduino_JSON.h>
// #include <PNGdec.h>
#include "pngle.h"
#include "keys/openweathermap_key.h"

#include "ParkingApp.h"

// main VGA Controller
fabgl::VGA16Controller* DisplayController;
fabgl::PS2Controller   PS2Controller;

// Weather
extern JSONVar myObject;
bool done_with_weather = false;
bool weather_succeeded = false;

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

void loop() {

  done_with_weather = true;
  weather_succeeded = getWeatherDB();

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





