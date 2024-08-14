#include <functional>
//#ifndef PARKSLOT_H
//#define PARKSLOT_H
#pragma once

#include "fabgl.h"
#include "fabui.h"
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
    uiLabel* slotUiLabel;

    // int taken;

    // int width;
    // int height;
    // int pos_x;
    // int pos_y;

    // int floorId;
    // int slotId;

    int selectedParkingType = TYPE_REGULAR;

    // callback function to be called when park slot choose button has been clicked
    std::function<void()> onChooseButtonClickCB;

    ParkSlot(fabgl::uiFrame* frameToSet, fabgl::Canvas* cvToSet, int x, int y, int width_t, int height_t, int floorId_t, int slotId_t,
             int selected_parking_type,std::function<void()> onChooseButtonClickCB_t)
        : frame(frameToSet), canvas(cvToSet), selectedParkingType(selected_parking_type)
    {

      // set callback function for Choose button clicked
      onChooseButtonClickCB = onChooseButtonClickCB_t;

      int taken = false;

      int width = width_t;
      int height = height_t;
      int pos_x = x;
      int pos_y = y;
      int type = 0;
      //String code;

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
        //code=db_parkingLot->floors[floorId].slots[slotId].slot_code;
      }

      ChooseButton = new uiCheckBox(frame, Point(pos_x, pos_y), Size(width, height), uiCheckBoxKind::RadioButton);
      

      // Add a label for the slot - floor id + slot id
      String slot_id_label = String(slotId);
      if(slotId < 10)
      {
        // In case the slot id is 1 digit number - add a zero before it
        slot_id_label = String("0") + slot_id_label;
      }
      
        
      String slot_label = String(floorId) + slot_id_label;
      slot_label = " " + slot_label + " ";
      const char* slot_label_cstr = slot_label.c_str();
      int slot_label_TextExt = calcWidthOfText(&fabgl::FONT_std_14, slot_label_cstr);

      slotUiLabel = new uiLabel(ChooseButton, slot_label_cstr, Point(width / 2 - slot_label_TextExt / 2 ,0), Size(slot_label_TextExt, 14), true);
      slotUiLabel->labelStyle().backgroundColor = RGB888(255,255,255);

      if(taken || (type != selectedParkingType && !(selectedParkingType == TYPE_DISABLED && type == TYPE_REGULAR)))
      {
        SetGroupTaken();
        ChooseButton->setChecked(true);
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
      // Set in 'taken' park slots group
      ChooseButton->setGroupIndex(-1);

      // Change the desired park slot color to 'taken' color 
      ChooseButton->checkBoxStyle().checkedBackgroundColor = TAKEN_COL;
      ChooseButton->checkBoxStyle().backgroundColor=TAKEN_COL;
      ChooseButton->checkBoxStyle().foregroundColor=TAKEN_COL;
      ChooseButton->checkBoxStyle().mouseOverBackgroundColor=TAKEN_COL;
      ChooseButton->checkBoxStyle().mouseOverForegroundColor=TAKEN_COL;

      // Set button uncheckable
      fabgl::uiWindowProps& props = ChooseButton->windowProps();
      props.activable = 0;
      props.focusable = 0;
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