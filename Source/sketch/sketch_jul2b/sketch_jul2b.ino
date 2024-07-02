#include "fabgl.h"
#include "bitmapsWarehouse.h"
// #include "fabui.h"
// #include <WiFi.h>

// #include "Bitmap_iot.h"
#include "ParkSlot.h"
#include "ParkingLot.h"

// main VGA Controller
fabgl::VGAController DisplayController;


// bitmap for parking image
Bitmap park_bitmap = Bitmap(92, 146, NBitmapsWarehouse::park, PixelFormat::RGBA2222);
Bitmap red_light_bitmap = Bitmap(60, 52, NBitmapsWarehouse::red_light, PixelFormat::RGBA2222);

// sprites (images) to show on the screen
Sprite sprites[4];

// main Canvas
fabgl::Canvas Canvas(&DisplayController);
//fabgl::uiApp runner;

const char* ssid = "Home12";
const char* password = "Dira1202";

// for animate test purposes 
int i;



void setup(){
  Serial.begin(115200);  // Start the serial communication

  i = 0;

  // Setup display controller
  DisplayController.begin();
  DisplayController.setResolution(VGA_720x400_70Hz); // VGA_640x350_70Hz

  // add sprites to display controller
  //DisplayController.setSprites(sprites, 4);
  // drawBitMap(&park_bitmap, sprite1_pos);

  // Initialize the UI application
  //runner.run(&DisplayController);
  //runner.run();


  // Wifi
  // Serial.begin(115200);
  // delay(1000);

  // WiFi.mode(WIFI_STA); //Optional
  // WiFi.begin(ssid, password);
  // Serial.println("\nConnecting");

  // while(WiFi.status() != WL_CONNECTED){
  //     Serial.print(".");
  //     delay(100);
  // }

  // Serial.println("\nConnected to the WiFi network");
  // Serial.print("Local ESP32 IP: ");
  // Serial.println(WiFi.localIP());

  // Sprites example
  // sprites[0].addBitmap(&red_light_bitmap);
  // sprites[0].moveTo(50, 30);
  // sprites[0].visible = true;
  // DisplayController.setSprites(sprites, 1);

  // set width of the pen used to draw lines \ rectangles 
  Canvas.setPenWidth(4);

  // set background
  Canvas.setBrushColor(Color::BrightWhite);
  Canvas.clear();

  // parking lot initiate
  ParkingLot parkingLot(&Canvas);

  // draw parking lot
  parkingLot.draw();

  // runner.runAsync(&DisplayController);
}

void loop() {
  Serial.print("i = ");
  Serial.println(i);

  // Clear the screen before drawing the sprite
  // Canvas.clear();

  // sprites[0].moveTo(50, 30 + y_offset);

  DisplayController.refreshSprites();

}
