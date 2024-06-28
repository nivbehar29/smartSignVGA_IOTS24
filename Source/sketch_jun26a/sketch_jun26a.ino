#include "fabgl.h"
#include "bitmapsWarehouse.h"
#include "fabui.h"
// #include <WiFi.h>

#include "Bitmap_iot.h"

// main VGA Controller
fabgl::VGAController DisplayController;

// main Canvas
fabgl::Canvas Canvas(&DisplayController);

// bitmap for parking image
Bitmap park_bitmap = Bitmap(92, 146, &NBitmapsWarehouse::park[0], PixelFormat::RGBA2222);

// sprites (images) to show on the screen
Sprite sprites[4];

fabgl::uiApp runner;

const char* ssid = "Home12";
const char* password = "Dira1202";

// for animate test purposes 
int i;

void drawBitMap_iot(Bitmap_iot bitmap) {
  Canvas.drawBitmap(bitmap.getPos().x, bitmap.getPos().y, bitmap.getBitmap());
}

void setup(){
  Serial.begin(115200);  // Start the serial communication

  i = 0;

  // Setup display controller
  DisplayController.begin();
  DisplayController.setResolution(VGA_640x350_70Hz);

  // add sprites to display controller
  //DisplayController.setSprites(sprites, 4);
  // drawBitMap(&park_bitmap, sprite1_pos);

  // Initialize the UI application
  //runner.run(&DisplayController);
  //runner.run();
  runner.runAsync(&DisplayController);

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
}

void loop() {
  Serial.print("i = ");
  Serial.println(i);
  
  int y_offset = 0;

  if(i == 2)
    y_offset = 100;
  else
    y_offset = 0;


  // Clear the screen before drawing the sprite
  Canvas.clear();

  // Draw the bitmaps
  Bitmap_iot bm1(&park_bitmap, 50, 30 + y_offset);
  Bitmap_iot bm2(&park_bitmap, 150, 30);
  Bitmap_iot bm3(&park_bitmap, 250, 30);
  Bitmap_iot bm4(&park_bitmap, 350, 30);

  drawBitMap_iot(bm1);
  drawBitMap_iot(bm2);
  drawBitMap_iot(bm3);
  drawBitMap_iot(bm4);



  i = (i + 1) % 3;

  delay(1000);

  // DisplayController.begin();
  // DisplayController.setResolution(VGA_640x350_70Hz);
  // fabgl::Canvas cv(&DisplayController);
  // cv.setPenColor(Color::Blue);
  // cv.drawText(0, 0, "best team IOT");
  // delay(30000);
  // ESP.restart();

}
