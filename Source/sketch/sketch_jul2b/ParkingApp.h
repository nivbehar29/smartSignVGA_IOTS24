#ifndef PARKINGAPP_H
#define PARKINGAPP_H

#include "fabgl.h"
#include "fabui.h"

class ParkingApp : public uiApp {

    private:

    uiButton* StartButton;

    uiFrame* OptionsFrame;

    uiLabel* WelcomeText;

    int ResX = 640;
    int ResY = 480;

    void init() {

      // set root window background color to dark green
      rootWindow()->frameStyle().backgroundColor = RGB888(0, 64, 0);

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

      // Start button
      int startTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Start");
      int buttonSizeX = startTextExt + 20;
      StartButton = new uiButton(rootWindow(), "Start", Point(ResX/2 - buttonSizeX/2, ResY/2), Size(buttonSizeX, 20));
      StartButton->onClick = [&]() { onStartButtonClick(); };

    }


  void onStartButtonClick() {

    OptionsFrame = new uiFrame(rootWindow(), "Options Frame", Point(0, 0), Size(ResX, ResY), true);

    // Frame properties
    fabgl::uiFrameProps& props = OptionsFrame->frameProps();
    props.hasCloseButton = 0;
    props.hasMinimizeButton = 0;
    props.hasMaximizeButton = 0;
    props.moveable = 0;
    props.resizeable = 0;

    // Cancel Button
    int CancelTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Cancel");
    Size CancelButtonSize(CancelTextExt + 20, 20);
    int y_top_offset = 10;
    int x_left_offset = 10;
    uiButton* CancelButton = new uiButton(OptionsFrame, "Cancel", Point(ResX - CancelButtonSize.width - x_left_offset, ResY - CancelButtonSize.height - y_top_offset), CancelButtonSize);
    CancelButton->onClick = [&]() { app()->showWindow(OptionsFrame, 0);
                                    OptionsFrame = nullptr;
                                  };



  }

  int calcWidthOfText(fabgl::FontInfo const * fontInfo, char const * text)
  {
    return app()->canvas()->textExtent(fontInfo, text);
  }
};

#endif // PARKINGAPP_H