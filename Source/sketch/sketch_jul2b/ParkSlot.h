#include <functional>
//#ifndef PARKSLOT_H
//#define PARKSLOT_H
#pragma once

#include "fabgl.h"
#include "GeneralFrame.h"
#include "ParkingLotFrame.h"
#include "FloorFrame.h"



class ParkSlot {


public:

    enum ParkStates {
        FREE,
        WAITING_FOR_VEHICLE,
        TAKEN
    };

    ParkStates state;
    fabgl::Canvas* canvas;
    fabgl::uiFrame* frame;
    uiCheckBox* ChooseButton;
    //parking_floor* Floorarr;
    int current_floor_id;
    int num_of_floors;
    //ParkingLotFrame* curr_frame;

    int width;
    int height;
    int x_pos;
    int y_pos;

    // callback function to be called when park slot choose button has been clicked
    std::function<void()> onChooseButtonClickCB;

    ParkSlot(fabgl::uiFrame* frameToSet, fabgl::Canvas* cvToSet, int x, int y, int width, int height, std::function<void()> onChooseButtonClickCB_t)
        : state(FREE), frame(frameToSet), canvas(cvToSet), x_pos(x), y_pos(y), width(width), height(height)
    {

      // set callback function for Choose button clicked
      onChooseButtonClickCB = onChooseButtonClickCB_t;

      int offset_y = 30;
      int ChooseButtonExt = calcWidthOfText(&fabgl::FONT_std_14, "Choose");
      Size ChooseButtonSize(ChooseButtonExt + 10, 20);
      ChooseButton= new uiCheckBox(frame, Point(x + width/2 - ChooseButtonSize.width/2, y + height - ChooseButtonSize.height - 5), Size(50, 120), uiCheckBoxKind::RadioButton);
      
      ChooseButton->setGroupIndex(1);
      ChooseButton->checkBoxStyle().backgroundColor = RGB888(200, 200, 200);
      ChooseButton->checkBoxStyle().mouseOverBackgroundColor = RGB888(0,0,255);
      ChooseButton->checkBoxStyle().checkedBackgroundColor = RGB888(0,128,0);
      ChooseButton->repaint();
      ChooseButton->onClick = [&]() {onChooseButtonClick();};
    }

    void SetGroupTaken()
    {
      // set in 'taken' park slots group
      ChooseButton->setGroupIndex(-1);

      // change the desired park slot color to 'taken' color 
      ChooseButton->checkBoxStyle().backgroundColor = RGB888(255,0,0);
      ChooseButton->checkBoxStyle().mouseOverBackgroundColor = RGB888(255,0,0);
      ChooseButton->checkBoxStyle().checkedBackgroundColor = RGB888(255,0,0);
    }
    
    void uncheckNotTaken() 
    {
      if(ChooseButton->groupIndex() != -1)
      {
          ChooseButton->setChecked(false);
      }
    }

    void setPos(int x, int y)
    {
      x_pos = x;
      y_pos = y;
    }

    int calcWidthOfText(fabgl::FontInfo const * fontInfo, char const * text)
    {
      return frame->app()->canvas()->textExtent(fontInfo, text);
    }

    void onChooseButtonClick()
    {
      if(this->ChooseButton->groupIndex() != -1)
      {
        // call callback function
        onChooseButtonClickCB();
      }
    }

};

//#endif // PARKSLOT_H