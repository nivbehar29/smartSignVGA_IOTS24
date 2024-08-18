#ifndef PARKINGAPP_H
#define PARKINGAPP_H

#include <Arduino_JSON.h>
#include <time.h>

#include "fabgl.h"
#include "fabui.h"

#include "OptionsFrame.h"
#include "ExitFrame.h"

#include "DBAux.h"
extern DB_parkingLot* db_parkingLot;

// extern JSONVar myObject;

class ParkingApp : public uiApp {

    public:

    ParkingApp(JSONVar* weatherObj_t) : weatherObj(weatherObj_t){}

    private:

    JSONVar* weatherObj;

    uiButton* StartButton;
    uiButton* ExitButton;

    OptionsFrame* optionsFrame;
    ExitFrame* exitFrame;

    uiLabel* WelcomeText;
    
    uiLabel* FlashingAdvText;
    uiTimerHandle FlashingAdvTimer;

    int MvoingAdvtextExt;
    uiLabel* MovingAdvText;
    uiTimerHandle MovingAdvTimer;

    uiTimerHandle FreeMemoryTimer;

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

      // frame where to put buttons
      // MainFrame = new uiFrame(rootWindow(), "", Point(10, 10), Size(115, 120));
      // MainFrame->frameStyle().backgroundColor = RGB888(0, 0, 255);
      // MainFrame->windowStyle().borderSize     = 0;

      // button to show TestControlsFrame
      optionsFrame = new OptionsFrame(rootWindow(), ResX, ResY, app());

      // Start button
      int startTextExt = calcWidthOfText(&fabgl::FONT_std_16, "Enter ");
      int startButtonSizeX = startTextExt + 30;
      StartButton = new uiButton(rootWindow(), "Enter ", Point(ResX/2 - startButtonSizeX/2, ResY/2 - 10), Size(startButtonSizeX, 30));
           StartButton->onClick = [&]() {  
                                      optionsFrame->showFrame();
                                      setActiveWindow(optionsFrame->frame);
                                    };
      StartButton->buttonStyle().textColor=RGB888(255,255,255);
      StartButton->buttonStyle().textFont = &fabgl::FONT_std_16;
      StartButton->buttonStyle().backgroundColor = RGB888(0,153,153);

      exitFrame = new ExitFrame(rootWindow(), ResX, ResY, app());
      
      // Exit button
      // int exitTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Exit");
      int exitButtonSizeX = startButtonSizeX;
      int startButtonHeight = 16;
      ExitButton = new uiButton(rootWindow(), "Leave ", Point(ResX/2 - exitButtonSizeX/2, ResY/2 + startButtonHeight + 10), Size(exitButtonSizeX, 30));
      ExitButton->buttonStyle().textFont = &fabgl::FONT_std_16;
      ExitButton->buttonStyle().textColor = RGB888(255,255,255);
      ExitButton->buttonStyle().backgroundColor = RGB888(153,0,0);
      //ExitButton->buttonStyle().textFont =
      ExitButton->onClick = [&]() {  
                                    exitFrame->showFrame();
                                    setActiveWindow(exitFrame->frame);
                                  };

      // Restart button
      int restartTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Restart");
      int restartButtonSizeX = restartTextExt + 20;
      uiButton* RestartButton = new uiButton(rootWindow(), "Restart", Point(10, 10), Size(restartButtonSizeX, 20));
      RestartButton->onClick = [&]() {
                                       ESP.restart();
                                     };

      if(weatherObj != nullptr)
      {
        int weather_city_label_text_ext = calcWidthOfText(&fabgl::FONT_std_16, "City: Haifa");
        uiLabel* weather_city_label = new uiLabel(rootWindow(), "City: Haifa", Point(ResX / 2 - weather_city_label_text_ext / 2, 10));

        String temperature_str = "Temperature: ";

        // uiLabel* weather_temperature_label = new uiLabel(rootWindow(), "Temperature:", Point(ResX / 2, 70));

        char buffer[20];  // Make sure this buffer is large enough to hold the converted value

        // dtostrf(float value, minimum width, precision, buffer)
        dtostrf(fahrenheitToCelcius((*weatherObj)["main"]["temp"]), 6, 2, buffer);  // Convert float to string with 2 decimal places

        temperature_str = temperature_str + String(buffer);
        int weather_temperature_info_text_ext = calcWidthOfText(&fabgl::FONT_std_16, temperature_str.c_str());


        uiLabel* weather_temperature_info = new uiLabel(rootWindow(), temperature_str.c_str(), Point(ResX / 2 - weather_temperature_info_text_ext / 2, 40));
      }

      // Flashing Advertisement Text
      char* flashing_adv = "Some Advertisement";
      if(db_parkingLot != nullptr && db_parkingLot->adv != nullptr)
        flashing_adv = db_parkingLot->adv;
      int FlashingAdvtextExt = calcWidthOfText(&fabgl::FONT_std_24, flashing_adv);
      FlashingAdvText = new uiLabel(rootWindow(), flashing_adv, Point(ResX / 2 - FlashingAdvtextExt / 2, 400));
      FlashingAdvText->labelStyle().backgroundColor = RGB888(0,0,64);
      FlashingAdvText->labelStyle().textFont        = &fabgl::FONT_std_24;
      FlashingAdvText->update();
      
      

      // Moving Advertisement Text
      char* moving_adv = "Some Moving Advertisement";
      if(db_parkingLot != nullptr && db_parkingLot->movingAdv != nullptr)
        moving_adv = db_parkingLot->movingAdv;
      MvoingAdvtextExt = calcWidthOfText(&fabgl::FONT_std_24, moving_adv);
      MovingAdvText = new uiLabel(rootWindow(), moving_adv, Point(ResX, 100));
      MovingAdvText->labelStyle().backgroundColor = RGB888(255,255,255);
      MovingAdvText->labelStyle().textFont        = &fabgl::FONT_std_24;
      MovingAdvText->update();
      

      this->onQuit =  [&]() { 
                              Serial.printf("onQuit() has been called !!!!!!!!");

                              optionsFrame->freeResources();
                              delete optionsFrame;
                            };
      
      this->onTimer = [&](uiTimerHandle tHandle) {  if (tHandle == FlashingAdvTimer) 
                                                      onFlashingAdvTimer(); 
                                                    
                                                    if(tHandle == MovingAdvTimer) 
                                                      onMovingAdvTimer();

                                                    if(tHandle == FreeMemoryTimer)
                                                    {
                                                      Serial.printf("Free 8bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_8BIT) / 1024);
                                                      Serial.printf("Free 32bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_32BIT) / 1024);
                                                    }
                                                      
                                                  };

      setTimers();

    }

    void setTimers()
    {
      FlashingAdvTimer = app()->setTimer(this, 500);
      MovingAdvTimer = app()->setTimer(this, 35);
      FreeMemoryTimer = app()->setTimer(this, 2000);
    }

    void stopTimers()
    {
      app()->killTimer(FlashingAdvTimer);
      app()->killTimer(MovingAdvTimer);
      app()->killTimer(FreeMemoryTimer);
    }

  void onFlashingAdvTimer()
  {
    // Serial.printf("Flash timer!\n");

    RGB888 curr_col = FlashingAdvText->labelStyle().backgroundColor;
    RGB888 next_col = curr_col;

    // some color manipulation for flashing
    next_col.B = next_col.B + 64;
    if (curr_col.B == 192)
    {
      next_col.G = next_col.G + 64;
      if (curr_col.G == 192) 
      {
        next_col.R = next_col.R + 64;
      }
    }

    FlashingAdvText->labelStyle().backgroundColor = next_col;
    FlashingAdvText->update();

    // set new timer
    //app()->killTimer(FlashingAdvTimer);
    //FlashingAdvTimer = app()->setTimer(this, 500);
  } 

  void onMovingAdvTimer()
  {
    // Serial.printf("Moving timer!\n");

    if(MovingAdvText->pos().X + MvoingAdvtextExt < 0)
    {
      app()->moveWindow(MovingAdvText, ResX, MovingAdvText->pos().Y);
    }
    else
    {
      app()->moveWindow(MovingAdvText, MovingAdvText->pos().X - 2, MovingAdvText->pos().Y);
    }
    //MovingAdvText->update();

    // set new timer
    //app()->killTimer(MovingAdvTimer);
    //MovingAdvTimer = app()->setTimer(this, 50);
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