//#ifndef FLOORFRAME_H
//#define FLOORFRAME_H

#pragma once

#include "fabgl.h"
#include "GeneralFrame.h"
#include "ParkSlot.h"
#include "canvas.h"

#include "string.h"

#include "DBAux.h"
extern DB_parkingLot* db_parkingLot;

class FloorFrame : public GeneralFrame {

public:

    int floor_id = 0;
    char floor_text[20];
    

    uiPaintBox * paintBox;
    Canvas* floor_canvas;

    bool isInitiated;
    ParkSlot** park_slots;
    int park_slots_num;
    int current_floor_id;
    int num_of_floors;
    std::function<void()> onParkSlotChooseButtonClickCB;

    FloorFrame(uiFrame * parent_t, int ResX_t, int ResY_t, uiApp* app_t, int floor_id_t,std::function<void()> onParkSlotChooseButtonClickCB_t)
    {
      parent = parent_t;
      ResX = ResX_t;
      ResY = ResY_t;
      app = app_t;
      floor_id = floor_id_t;
      onParkSlotChooseButtonClickCB = onParkSlotChooseButtonClickCB_t;

      isInitiated = false;

      park_slots = nullptr;
      if(db_parkingLot != nullptr)
      {
        park_slots_num = db_parkingLot->floors[floor_id].num_slots;
      }
      else
      {
        Serial.println("db_parkingLot is null!");
        park_slots_num = 5;
      }

      sprintf(floor_text, "floor %d", floor_id);

      frame = new uiFrame(parent, floor_text, Point(65, 20), Size(ResX - 70, ResY-30), false);

      // Frame properties
      fabgl::uiFrameProps& props = frame->frameProps();
      props.hasCloseButton = 0;
      props.hasMinimizeButton = 0;
      props.hasMaximizeButton = 0;
      props.moveable = 0;
      props.resizeable = 0;
    
      // Floor text
      new uiLabel(frame, floor_text, Point(0,0));

      paintBox = new uiPaintBox(frame, frame->clientPos(), frame->clientSize());
      paintBox->paintBoxStyle().backgroundColor = Color::Black;
      paintBox->onPaint = [&](Rect const & r) { FloorPaint(); };
    }

    void showFrame()
    {
      app->showWindow(frame, 1);
      FloorPaint();
    }

    void FloorPaint()
    {
      // initiate if this is the first time we paint it
      if(!isInitiated)
      {
        // init canvas
        floor_canvas = frame->canvas();
        //park_slot = new ParkSlot(frame, floor_canvas, 10, 10, 50, 100);
        park_slots = (ParkSlot**) malloc(sizeof(ParkSlot*) * park_slots_num);

        int offset_x = 10;
        for(int i = 0; i < park_slots_num; i++)
        {
          int width = 50;
          park_slots[i] = new ParkSlot(frame, floor_canvas, offset_x, 10, width, 100, floor_id, i, onParkSlotChooseButtonClickCB);
          offset_x += width + 5;
        }

        isInitiated = true;
      }
    }

    void uncheckParkingSlots() {
      for(int i = 0; i < park_slots_num; i++)
      {
        if(park_slots != nullptr )
        {
           park_slots[i]->uncheckNotTaken();
        }
      }
    }

     int IfCheckedSetInTakenGroup()
     {
        for(int i = 0; i < park_slots_num; i++)
        {
          if(park_slots != nullptr && park_slots[i] != nullptr &&
             park_slots[i]->ChooseButton->checked() && park_slots[i]->ChooseButton->groupIndex() == 1)
          {
            // set in taken park slots group
            park_slots[i]->SetGroupTaken();
            return i;
          }    
        }

        return -1;
     }
};


//#endif // FLOORFRAME_H