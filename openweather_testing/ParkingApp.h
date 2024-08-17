#ifndef PARKINGAPP_H
#define PARKINGAPP_H

#include <Arduino_JSON.h>
#include <time.h>

#include "fabgl.h"
#include "fabui.h"

// extern JSONVar myObject;

class ParkingApp : public uiApp {

    public:

    ParkingApp(JSONVar* weatherObj_t) : weatherObj(weatherObj_t){}

    private:

    JSONVar* weatherObj;
    uiLabel* WelcomeText;

    int ResX = 640;
    int ResY = 480;

    void init() {

      // set root window background color to dark green
      // 1000 1100 1100 0001 1111 1111
      // color picker: https://codepen.io/kevinli/pen/GRpXOvo
      rootWindow()->frameStyle().backgroundColor = RGB888(0, 0, 0);

      // Welcome Text
      int textExt = calcWidthOfText(&fabgl::FONT_std_24, "Welcome To The Parking Lot!");
      WelcomeText = new uiLabel(rootWindow(), "Welcome To The Parking Lot!", Point(ResX / 2 - textExt / 2, 160));
      WelcomeText->labelStyle().backgroundColor = rootWindow()->frameStyle().backgroundColor;
      WelcomeText->labelStyle().textColor =RGB888(255,255,255);
      WelcomeText->labelStyle().textFont = &fabgl::FONT_10x20 ;
      //WelcomeText->labelStyle().textFont        = &fabgl::FONT_std_24;
      WelcomeText->update();


      // Restart button
      int restartTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Restart");
      int restartButtonSizeX = restartTextExt + 20;
      uiButton* RestartButton = new uiButton(rootWindow(), "Restart", Point(10, 10), Size(restartButtonSizeX, 20));
      RestartButton->onClick = [&]() {
                                       ESP.restart();
                                     };

      if(weatherObj != nullptr)
      {
        uiLabel* weather_city_label = new uiLabel(rootWindow(), "City:", Point(ResX / 2 + 200, 10));
        uiLabel* weather_city_info = new uiLabel(rootWindow(), "Haifa", Point(ResX / 2 + 200 , 40));
        uiLabel* weather_temperature_label = new uiLabel(rootWindow(), "Temperature:", Point(ResX / 2 + 200 , 70));

        char buffer[20];  // Make sure this buffer is large enough to hold the converted value

        // dtostrf(float value, minimum width, precision, buffer)
        dtostrf(fahrenheitToCelcius((*weatherObj)["main"]["temp"]), 6, 2, buffer);  // Convert float to string with 2 decimal places

        uiLabel* weather_temperature_info = new uiLabel(rootWindow(), buffer, Point(ResX / 2 + 200 , 100));
      }

    }

  int calcWidthOfText(fabgl::FontInfo const * fontInfo, char const * text)
  {
    return app()->canvas()->textExtent(fontInfo, text);
  }

  double fahrenheitToCelcius(double c)
  {
    return c - 273.15;
  }

};

#endif // PARKINGAPP_H