#ifndef OPTIONSFRAME_H
#define OPTIONSFRAME_H

#include "fabgl.h"
#include "fabui.h"
#include "GeneralFrame.h"
#include "ParkingLotFrame.h"

class OptionsFrame : public GeneralFrame {

  public:

  uiCheckBox* disabledCB;
  uiCheckBox* electricCarCB;
  uiCheckBox* motorcycleCB;

  uiLabel* TimesliderLabel;
  uiSlider* Timeslider1;

  ParkingLotFrame* parkingLotFrame;

  std::function<void()> onFinishButtonClickCB;

  OptionsFrame(uiFrame * parent_t, int ResX_t, int ResY_t, uiApp* app_t, std::function<void()> onFinishButtonClickCB_t)
  {
    parent = parent_t;
    ResX = ResX_t;
    ResY = ResY_t;
    app = app_t;
    onFinishButtonClickCB = onFinishButtonClickCB_t;

    frame = new uiFrame(parent, "Options Frame", Point(0, 0), Size(ResX, ResY), false);

    // Frame properties
    fabgl::uiFrameProps& props = frame->frameProps();
    props.hasCloseButton = 0;
    props.hasMinimizeButton = 0;
    props.hasMaximizeButton = 0;
    props.moveable = 0;
    props.resizeable = 0;

    // Cancel Button
    int CancelTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Cancel");
    Size CancelButtonSize(CancelTextExt + 20, 20);
    int cancel_y_top_offset = 10;
    int cancel_x_left_offset = 10;
    uiButton* CancelButton = new uiButton(frame, "Cancel", Point(cancel_x_left_offset, ResY - CancelButtonSize.height - cancel_y_top_offset), CancelButtonSize);
    CancelButton->onClick = [&]() { app->showWindow(frame, 0); };


    parkingLotFrame = new ParkingLotFrame(parent, ResX, ResY, app, 5, onFinishButtonClickCB);

    // Next Button
    int NextTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Next");
    Size NextButtonSize(NextTextExt + 20, 20);
    int next_y_top_offset = 10;
    int next_x_left_offset = 10;
    uiButton* NextButton = new uiButton(frame, "Next", Point(ResX - NextButtonSize.width - next_x_left_offset, ResY - NextButtonSize.height - next_y_top_offset), NextButtonSize);
    NextButton->onClick = [&]() { 
                                    app->showWindow(frame, 0);
                                    app->showWindow(parkingLotFrame->frame, 1);
                                    app->setActiveWindow(parkingLotFrame->frame); 
                                };

    // Setup Checkboxes
    int cbSizeXY = 55;
    int cbLeftOffset = 20;
    int cbTextLeftOffset = 10;
    int labelsFontHeight = (&fabgl::FONT_std_24)->height;
    int cbTextPosX = cbLeftOffset + cbSizeXY + cbTextLeftOffset;

    uiStaticLabel* disabledCBLabel =     new uiStaticLabel(frame, "Disabled",      Point(cbTextPosX,  50 + cbSizeXY/2 - labelsFontHeight/2));
    uiStaticLabel* electricCarCBLabel =  new uiStaticLabel(frame, "Electric Car",  Point(cbTextPosX,  50 + cbSizeXY + cbSizeXY/2 + cbSizeXY/2 - labelsFontHeight/2));
    uiStaticLabel* motorcycleCBLabel =   new uiStaticLabel(frame, "Motorcycle",    Point(cbTextPosX,  50 + cbSizeXY*3 + cbSizeXY/2 - labelsFontHeight/2));

    disabledCBLabel->labelStyle().textFont =    &fabgl::FONT_std_24;
    electricCarCBLabel->labelStyle().textFont = &fabgl::FONT_std_24;
    motorcycleCBLabel->labelStyle().textFont =  &fabgl::FONT_std_24;

    disabledCBLabel->update();
    electricCarCBLabel->update();
    motorcycleCBLabel->update();

    disabledCB = new uiCheckBox(frame,    Point(cbLeftOffset, 50),                          Size(cbSizeXY, cbSizeXY));
    electricCarCB = new uiCheckBox(frame, Point(cbLeftOffset, 50 + cbSizeXY + cbSizeXY/2),  Size(cbSizeXY, cbSizeXY));
    motorcycleCB = new uiCheckBox(frame,  Point(cbLeftOffset, 50 + cbSizeXY*3),             Size(cbSizeXY, cbSizeXY));

    // Time slider
    TimesliderLabel = new uiLabel(frame, "0", Point(ResX/2 + 100,50));
    Timeslider1 = new uiSlider(frame, Point(ResX/2 +30 ,50), Size(17, 400), uiOrientation::Vertical);
    Timeslider1->anchors().right = true;
    Timeslider1->setup(0, 24, 1);
    Timeslider1->setPosition(20);
    Timeslider1->onChange = [&]() {
                                      int val = Timeslider1->position();
                                      int hours = val;
                                      TimesliderLabel->setTextFmt("hours = %d",hours);
                                      TimesliderLabel->update();
                                  };

  }
};

#endif // OPTIONSFRAME_H