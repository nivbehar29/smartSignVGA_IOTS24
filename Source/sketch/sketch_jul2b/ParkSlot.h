#include <functional>
//#ifndef PARKSLOT_H
//#define PARKSLOT_H
#pragma once

#include "fabgl.h"
#include "GeneralFrame.h"
#include "ParkingLotFrame.h"
#include "FloorFrame.h"

#include "DBAux.h"
extern DB_parkingLot* db_parkingLot;

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

    int taken;

    int width;
    int height;
    int pos_x;
    int pos_y;

    int floorId;
    int slotId;

    // callback function to be called when park slot choose button has been clicked
    std::function<void()> onChooseButtonClickCB;

    ParkSlot(fabgl::uiFrame* frameToSet, fabgl::Canvas* cvToSet, int x, int y, int width, int height, int floorId, int slotId, std::function<void()> onChooseButtonClickCB_t)
        : state(FREE), frame(frameToSet), canvas(cvToSet), pos_x(x), pos_y(y), width(width), height(height), floorId(floorId), slotId(slotId)
    {

      // set callback function for Choose button clicked
      onChooseButtonClickCB = onChooseButtonClickCB_t;

      taken = false;

      if(db_parkingLot != nullptr)
      {
        taken = db_parkingLot->floors[floorId].slots[slotId].is_taken;
        pos_x = db_parkingLot->floors[floorId].slots[slotId].pos_x;
        pos_y = db_parkingLot->floors[floorId].slots[slotId].pos_y;
      }

      // int offset_y = 30;
      // int ChooseButtonExt = calcWidthOfText(&fabgl::FONT_std_14, "Choose");
      // Size ChooseButtonSize(ChooseButtonExt + 10, 20);
      // ChooseButton = new uiCheckBox(frame, Point(x + width/2 - ChooseButtonSize.width/2, y + height - ChooseButtonSize.height - 5), Size(50, 120), uiCheckBoxKind::RadioButton);
      ChooseButton = new uiCheckBox(frame, Point(pos_x, pos_y), Size(50, 120), uiCheckBoxKind::RadioButton);
      
      if(taken)
      {
        SetGroupTaken();
        ChooseButton->setChecked(true);
      }
      else
      {
        ChooseButton->setGroupIndex(1);
        ChooseButton->checkBoxStyle().checkedBackgroundColor=RGB888(0,128,0);
      }
      
      ChooseButton->checkBoxStyle().mouseOverBackgroundColor=RGB888(0,0,0);
      ChooseButton->checkBoxStyle().foregroundColor=RGB888(0,128,0);
      ChooseButton->repaint();
      ChooseButton->onClick = [&]() {onChooseButtonClick();};
    }

    void SetGroupTaken()
    {
      // set in 'taken' park slots group
      ChooseButton->setGroupIndex(-1);

      // change the desired park slot color to 'taken' color 
      ChooseButton->checkBoxStyle().checkedBackgroundColor = RGB888(128,0,0);
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
      pos_x = x;
      pos_y = y;
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