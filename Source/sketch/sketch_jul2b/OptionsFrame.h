#ifndef OPTIONSFRAME_H
#define OPTIONSFRAME_H

#include "fabgl.h"
#include "fabui.h"
#include "GeneralFrame.h"
#include "ParkingLotFrame.h"
#include "Bitmaps.h"

class OptionsFrame : public GeneralFrame {

  public:

  int selectedParkingType = TYPE_REGULAR;
  uiCheckBox* regularCB;
  uiCheckBox* disabledCB;
  uiCheckBox* electricCarCB;
  uiCheckBox* motorcycleCB;

  uiLabel* TimesliderLabel;
  uiSlider* Timeslider1;

  ParkingLotFrame* parkingLotFrame;

  OptionsFrame(uiFrame * parent_t, int ResX_t, int ResY_t, uiApp* app_t )
  {
    parent = parent_t;
    ResX = ResX_t;
    ResY = ResY_t;
    app = app_t;

    frame = new uiFrame(parent, "", Point(0, 0), Size(ResX, ResY), false);

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
    int cancel_y_top_offset = 40;
    int cancel_x_left_offset = 10;
    uiButton* CancelButton = new uiButton(frame, "Cancel", Point(cancel_x_left_offset,CancelButtonSize.height + 5), CancelButtonSize);
    CancelButton->onClick = [&]() { app->showWindow(frame, 0); };


    parkingLotFrame = new ParkingLotFrame(parent, ResX, ResY, app, 5);

    // Next Button
    int NextTextExt = calcWidthOfText(&fabgl::FONT_std_16, "Next");
    Size NextButtonSize(NextTextExt + 20, 20);
    int next_y_top_offset = 40;
    int next_x_left_offset = 10;
    uiButton* NextButton = new uiButton(frame, "Next", Point(ResX - NextButtonSize.width - next_x_left_offset, NextButtonSize.height + 5), NextButtonSize);
    NextButton->buttonStyle().textColor=RGB888(255,255,255);
    NextButton->buttonStyle().textFont = &fabgl::FONT_std_16;
    NextButton->buttonStyle().backgroundColor = RGB888(0,153,153);
    NextButton->onClick = [&]() { onNextButtonClick();};

    // Setup Checkboxes
    int cbSizeXY = 55;
    int cbLeftOffset = 20;
    int cbTextLeftOffset = 10;
    int labelsFontHeight = (&fabgl::FONT_std_24)->height;
    int cbTextPosX = cbLeftOffset + cbSizeXY + cbTextLeftOffset + cbSizeXY ;


    Point regularCBPos = Point(cbTextPosX,  50 + cbSizeXY/2 - labelsFontHeight/2);
    Point disabledCBPos = Point(cbTextPosX,  50 + cbSizeXY + cbSizeXY/2 + cbSizeXY/2 - labelsFontHeight/2);
    Point electricCarCBPos = Point(cbTextPosX,  50 + cbSizeXY*3 + cbSizeXY/2 - labelsFontHeight/2);
    Point motorcycleCBPos = Point(cbTextPosX,  50 + cbSizeXY*5 - labelsFontHeight/2);



    uiStaticLabel* regularCBLabel =      new uiStaticLabel(frame, "Regular",       Point(cbTextPosX,  50 + cbSizeXY/2 - labelsFontHeight/2 ));
    uiStaticLabel* disabledCBLabel =     new uiStaticLabel(frame, "Disabled",      Point(cbTextPosX+300,  50 + cbSizeXY/2 - labelsFontHeight/2 ));
    uiStaticLabel* electricCarCBLabel =  new uiStaticLabel(frame, "Electric Car",  Point(cbTextPosX,  50 + cbSizeXY*3 + cbSizeXY/2 - labelsFontHeight/2));
    uiStaticLabel* motorcycleCBLabel =   new uiStaticLabel(frame, "Motorcycle",    Point(cbTextPosX+300,  50 + cbSizeXY*3 + cbSizeXY/2 - labelsFontHeight/2));

    regularCBLabel->labelStyle().textFont =     &fabgl::FONT_std_24;
    disabledCBLabel->labelStyle().textFont =    &fabgl::FONT_std_24;
    electricCarCBLabel->labelStyle().textFont = &fabgl::FONT_std_24;
    motorcycleCBLabel->labelStyle().textFont =  &fabgl::FONT_std_24;

    regularCBLabel->update();
    disabledCBLabel->update();
    electricCarCBLabel->update();
    motorcycleCBLabel->update();

    // Parking type radio buttons
    regularCB =     new uiCheckBox(frame, Point(cbLeftOffset, 50),                            Size(cbSizeXY, cbSizeXY), uiCheckBoxKind::RadioButton);
    disabledCB =    new uiCheckBox(frame, Point(cbLeftOffset+300, 50 ),    Size(cbSizeXY, cbSizeXY), uiCheckBoxKind::RadioButton);
    electricCarCB = new uiCheckBox(frame, Point(cbLeftOffset, 50 + cbSizeXY*3),               Size(cbSizeXY, cbSizeXY), uiCheckBoxKind::RadioButton);
    motorcycleCB =  new uiCheckBox(frame, Point(cbLeftOffset+300, 50 + cbSizeXY*3),  Size(cbSizeXY, cbSizeXY), uiCheckBoxKind::RadioButton);

    // Parking type icons
    uiImage* regularImage = new uiImage(frame, &regular_car_bitmap, Point(cbLeftOffset + cbSizeXY + 10, 50));
    uiImage* disabledImage = new uiImage(frame, &disabled_car_bitmap, Point(cbLeftOffset + cbSizeXY + 10 + 300, 50));
    uiImage* electricCarImage = new uiImage(frame, &electric_car_bitmap, Point(cbLeftOffset + cbSizeXY + 10, 50 + cbSizeXY*3));
    uiImage* motorcycleImage = new uiImage(frame, &motorcycle_bitmap, Point(cbLeftOffset + cbSizeXY + 10 + 300, 50 + cbSizeXY*3));

    // Set radio buttons to be on the same group
    regularCB->setGroupIndex(2);
    disabledCB->setGroupIndex(2);
    electricCarCB->setGroupIndex(2);
    motorcycleCB->setGroupIndex(2);

    // Define what happens when clicking on the buttons
    regularCB->onClick = [&]()      {selectedParkingType = TYPE_REGULAR;};
    disabledCB->onClick = [&]()     {selectedParkingType = TYPE_DISABLED;};
    electricCarCB->onClick = [&]()  {selectedParkingType = TYPE_ELECTRIC;};
    motorcycleCB->onClick = [&]()   {selectedParkingType = TYPE_MOTORCYCLE;};
  }

  void showFrame()
  {
    selectedParkingType = TYPE_REGULAR;
    regularCB->setChecked(true);
    app->showWindow(frame, 1);
  }

  void onNextButtonClick()
  {
    // Hide current windows
    app->showWindow(frame, 0);

    // Notify the next window abut the chosen parking type 
    Serial.println("OptionsFrame: NextClick: selected parking type is " + String(selectedParkingType));
    parkingLotFrame->setSelectedParkingType(selectedParkingType);

    // Show the next window and set it as active window
    parkingLotFrame->showFrame();
  }

  void freeResources()
  {
    parkingLotFrame->freeResources();
    delete parkingLotFrame;
  }
};

#endif // OPTIONSFRAME_H