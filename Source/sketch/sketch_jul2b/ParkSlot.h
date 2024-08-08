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

#define TAKEN_COL (RGB888(128,0,0))
#define CLICKED_COL (RGB888(0,128,0))
#define MOUSE_OVER_COL (RGB888(40,93,255))
#define FREE_COL (RGB888(128,128,128))

class ParkSlot {


public:

    enum ParkStates {
        FREE,
        WAITING_FOR_VEHICLE,
        TAKEN
    };

    // ParkStates state;
    fabgl::Canvas* canvas;
    fabgl::uiFrame* frame;
    uiCheckBox* ChooseButton;

    // int taken;

    // int width;
    // int height;
    // int pos_x;
    // int pos_y;

    // int floorId;
    // int slotId;

    // callback function to be called when park slot choose button has been clicked
    std::function<void()> onChooseButtonClickCB;

    ParkSlot(fabgl::uiFrame* frameToSet, fabgl::Canvas* cvToSet, int x, int y, int width_t, int height_t, int floorId_t, int slotId_t, std::function<void()> onChooseButtonClickCB_t)
        : /*state(FREE),*/ frame(frameToSet), canvas(cvToSet)/*, pos_x(x), pos_y(y), width(width), height(height), floorId(floorId), slotId(slotId)*/
    {

      // set callback function for Choose button clicked
      onChooseButtonClickCB = onChooseButtonClickCB_t;

      int taken = false;

      int width = width_t;
      int height = height_t;
      int pos_x = x;
      int pos_y = y;
      int type = 0;

      int floorId = floorId_t;
      int slotId = slotId_t;

      if(db_parkingLot != nullptr)
      {
        taken = db_parkingLot->floors[floorId].slots[slotId].is_taken;
        pos_x = db_parkingLot->floors[floorId].slots[slotId].pos_x;
        pos_y = db_parkingLot->floors[floorId].slots[slotId].pos_y;
        width = db_parkingLot->floors[floorId].slots[slotId].width;
        height = db_parkingLot->floors[floorId].slots[slotId].height;
        type = db_parkingLot->floors[floorId].slots[slotId].type;
      }


      // int offset_y = 30;
      String type_str;
      if(type == TYPE_REGULAR)
        type_str = "Regular";
      else if(type == TYPE_DISABLED)
        type_str = "Disabled";
      else if(type == TYPE_ELECTRIC)
        type_str = "Electric";
      else if(type == TYPE_MOTORCYCLE)
        type_str = "Motor";
      const char* type_cstr = type_str.c_str();
      int typeTextExt = calcWidthOfText(&fabgl::FONT_std_14, type_cstr);
      // Size ChooseButtonSize(ChooseButtonExt + 10, 20);
      // ChooseButton = new uiCheckBox(frame, Point(x + width/2 - ChooseButtonSize.width/2, y + height - ChooseButtonSize.height - 5), Size(50, 120), uiCheckBoxKind::RadioButton);
      ChooseButton = new uiCheckBox(frame, Point(pos_x, pos_y), Size(width, height), uiCheckBoxKind::RadioButton);

      // add label for the type of the parking slot
      // 1 - regular
      // 2 - disabled
      // 3 - electric
      // 4 - motorcycle
      uiLabel* u = new uiLabel(ChooseButton, type_cstr, Point(0, 0), Size(typeTextExt, 14), true);
      
      if(taken)
      {
        SetGroupTaken();
        ChooseButton->setChecked(true);
        ChooseButton->checkBoxStyle().checkedBackgroundColor=TAKEN_COL;
        ChooseButton->checkBoxStyle().backgroundColor=TAKEN_COL;
        ChooseButton->checkBoxStyle().foregroundColor=TAKEN_COL; 
        ChooseButton->checkBoxStyle().mouseOverBackgroundColor=TAKEN_COL;
        ChooseButton->checkBoxStyle().mouseOverForegroundColor=TAKEN_COL;
      }
      else
      {
        ChooseButton->setGroupIndex(1);
        ChooseButton->checkBoxStyle().checkedBackgroundColor=CLICKED_COL;
        ChooseButton->checkBoxStyle().backgroundColor=FREE_COL;
        ChooseButton->checkBoxStyle().foregroundColor=FREE_COL; 
        ChooseButton->checkBoxStyle().mouseOverBackgroundColor=MOUSE_OVER_COL;
        ChooseButton->checkBoxStyle().mouseOverForegroundColor=MOUSE_OVER_COL;
      }
      
      // ChooseButton->checkBoxStyle().foregroundColor=RGB888(0,128,0);
      ChooseButton->repaint();
      ChooseButton->onClick = [&]() {onChooseButtonClick();};
    }

    void SetGroupTaken()
    {
      // set in 'taken' park slots group
      ChooseButton->setGroupIndex(-1);

      // change the desired park slot color to 'taken' color 
      ChooseButton->checkBoxStyle().checkedBackgroundColor = TAKEN_COL;
      ChooseButton->checkBoxStyle().foregroundColor=TAKEN_COL;
      ChooseButton->checkBoxStyle().mouseOverBackgroundColor=TAKEN_COL;
      ChooseButton->checkBoxStyle().mouseOverForegroundColor=TAKEN_COL;
    }
    
    void uncheckNotTaken() 
    {
      if(ChooseButton->groupIndex() != -1)
      {
          ChooseButton->setChecked(false);
          ChooseButton->checkBoxStyle().backgroundColor=FREE_COL;
          ChooseButton->checkBoxStyle().foregroundColor=FREE_COL; 
          ChooseButton->checkBoxStyle().mouseOverBackgroundColor=MOUSE_OVER_COL;
          ChooseButton->checkBoxStyle().mouseOverForegroundColor=MOUSE_OVER_COL;
      }
    }

    // void setPos(int x, int y)
    // {
    //   pos_x = x;
    //   pos_y = y;
    // }

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

        ChooseButton->checkBoxStyle().foregroundColor=CLICKED_COL; 
      }
    }
};

//#endif // PARKSLOT_H