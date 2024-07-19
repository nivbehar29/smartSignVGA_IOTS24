#ifndef PARKINGAPP_H
#define PARKINGAPP_H

#include <Arduino_JSON.h>
#include <time.h>

#include "fabgl.h"
#include "fabui.h"

#include "OptionsFrame.h"

// extern JSONVar myObject;

class ParkingApp : public uiApp {

    public:

    ParkingApp(JSONVar* weatherObj_t) : weatherObj(weatherObj_t) {}

    private:

    JSONVar* weatherObj;

    uiButton* StartButton;

    OptionsFrame* optionsFrame;

    uiLabel* WelcomeText;
    uiLabel* AdvText;
    uiTimerHandle timer;

    int ResX = 640;
    int ResY = 480;

    void init() {

      // set root window background color to dark green
      rootWindow()->frameStyle().backgroundColor = RGB888(0, 64, 0);

      timer = nullptr;

      // Welcome Text
      int textExt = calcWidthOfText(&fabgl::FONT_std_24, "Welcome To The Parking Lot!");
      WelcomeText = new uiLabel(rootWindow(), "Welcome To The Parking Lot!", Point(ResX / 2 - textExt / 2, 150));
      WelcomeText->labelStyle().backgroundColor = rootWindow()->frameStyle().backgroundColor;
      WelcomeText->labelStyle().textFont        = &fabgl::FONT_std_24;
      WelcomeText->update();

      // frame where to put buttons
      // MainFrame = new uiFrame(rootWindow(), "", Point(10, 10), Size(115, 120));
      // MainFrame->frameStyle().backgroundColor = RGB888(0, 0, 255);
      // MainFrame->windowStyle().borderSize     = 0;

      // button to show TestControlsFrame
      optionsFrame = new OptionsFrame(rootWindow(), ResX, ResY, app());

      // Start button
      int startTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Start");
      int buttonSizeX = startTextExt + 20;
      StartButton = new uiButton(rootWindow(), "Start", Point(ResX/2 - buttonSizeX/2, ResY/2), Size(buttonSizeX, 20));
      StartButton->onClick = [&]() { showWindow(optionsFrame->frame, true); setActiveWindow(optionsFrame->frame); };

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

      // Advertisement Text
      int AdvtextExt = calcWidthOfText(&fabgl::FONT_std_24, "Some Advertisement !!!");
      AdvText = new uiLabel(rootWindow(), "Some Advertisement !!!", Point(ResX / 2 - AdvtextExt / 2, 400));
      AdvText->labelStyle().backgroundColor = rootWindow()->frameStyle().backgroundColor;
      AdvText->labelStyle().textFont        = &fabgl::FONT_std_24;
      AdvText->update();
      

      timer = app()->setTimer(this, 1000);
      this->onTimer = [&](uiTimerHandle tHandle) {  Serial.println("onTimer");

                                                    if(AdvText->labelStyle().backgroundColor == RGB888(0, 64, 0))
                                                      AdvText->labelStyle().backgroundColor = RGB888(64, 64, 64);
                                                    else
                                                      AdvText->labelStyle().backgroundColor = RGB888(0, 64, 0);
                                                    AdvText->update();

                                                    // set new timer
                                                    app()->killTimer(timer);
                                                    timer = nullptr;
                                                    timer = app()->setTimer(this, 1000);
                                                    /*app()->canvas->repaint();*/ };


    }


  // void onStartButtonClick() {

  //   OptionsFrame = new uiFrame(rootWindow(), "Options Frame", Point(0, 0), Size(ResX, ResY), true);

  //   // Frame properties
  //   fabgl::uiFrameProps& props = OptionsFrame->frameProps();
  //   props.hasCloseButton = 0;
  //   props.hasMinimizeButton = 0;
  //   props.hasMaximizeButton = 0;
  //   props.moveable = 0;
  //   props.resizeable = 0;

  //   // Cancel Button
  //   int CancelTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Cancel");
  //   Size CancelButtonSize(CancelTextExt + 20, 20);
  //   int y_top_offset = 10;
  //   int x_left_offset = 10;
  //   uiButton* CancelButton = new uiButton(OptionsFrame, "Cancel", Point(ResX - CancelButtonSize.width - x_left_offset, ResY - CancelButtonSize.height - y_top_offset), CancelButtonSize);
  //   CancelButton->onClick = [&]() { app()->showWindow(OptionsFrame, 0);
  //                                   OptionsFrame = nullptr;
  //                                 };



  // }

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