//#ifndef PARKINGLOTFRAME_H
//#define PARKINGLOTFRAME_H

#pragma once
#include "fabgl.h"
#include "FloorFrame.h"
#include "string.h"
#include <HardwareSerial.h>
HardwareSerial mySerial(1);
//#include "ParkSlot.h"
#include "DBAux.h"
extern DB_parkingLot* db_parkingLot;


struct parking_floor {
  int num_of_regular_spaces;
  int num_of_disabled_spaces;
  int num_of_electric_spaces;
  int num_of_motor_spaces;
  char floor_text[20];
  uiStaticLabel* floor_label;
  uiCheckBox* floor_box;
  FloorFrame* floor_frame;
};

class ParkingLotFrame : public GeneralFrame {

public:
  int num_of_floors;
  parking_floor* FloorArr;
  int current_floor_id;


  ParkingLotFrame(uiFrame* parent_t, int ResX_t, int ResY_t, uiApp* app_t, int FloorsNum) {

    if (db_parkingLot != nullptr) {
      num_of_floors = db_parkingLot->num_floors;
    } else {
      Serial.println("db_parkingLot is null!");
      num_of_floors = FloorsNum;
    }

    parent = parent_t;
    ResX = ResX_t;
    ResY = ResY_t;
    app = app_t;
    frame = new uiFrame(parent, "Parking Lot Frame", Point(0, 0), Size(ResX, ResY), false);

    FloorArr = (parking_floor*)malloc(num_of_floors * sizeof(parking_floor));

    // Frame properties
    fabgl::uiFrameProps& props = frame->frameProps();
    props.hasCloseButton = 0;
    props.hasMinimizeButton = 0;
    props.hasMaximizeButton = 0;
    props.moveable = 0;
    props.resizeable = 0;

    // Finish Button
    int FinishTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Finish");
    Size FinishButtonSize(FinishTextExt + 20, 20);
    int cancel_y_top_offset = 10;
    int cancel_x_left_offset = 10;
    uiButton* FinishButton = new uiButton(frame, "Finish", Point(cancel_x_left_offset, ResY - FinishButtonSize.height - cancel_y_top_offset), FinishButtonSize);
    FinishButton->onClick = [&]() {
      onFinishButtonClicked();
    };

    //
    for (int i = 0; i < num_of_floors; i++)
    {
      FloorArr[i].num_of_regular_spaces = 10;
      FloorArr[i].num_of_electric_spaces = 5;
      FloorArr[i].num_of_disabled_spaces = 2;
      FloorArr[i].num_of_motor_spaces = 2;
      sprintf(FloorArr[i].floor_text, "Floor %d", i);

      int offset_y = 30;
      FloorArr[i].floor_label = new uiStaticLabel(frame, FloorArr[i].floor_text, Point(10, i * 40 + offset_y));
      FloorArr[i].floor_box = new uiCheckBox(frame, Point(45, i * 40 + offset_y), Size(16, 16), uiCheckBoxKind::RadioButton);
      FloorArr[i].floor_box->setGroupIndex(1);
      FloorArr[i].floor_frame = nullptr;

      FloorArr[i].floor_box->onClick = [&, i]() {
        onFloorCheckboxClick(i);
      };
    }

    current_floor_id = -1;

    // Activate first floor by default
    // FloorArr[0].floor_box->onClick();
    // FloorArr[0].floor_box->setChecked(true);
    // onFloorCheckboxClick(0);
    // current_floor_id = 0;
  }

  void onFinishButtonClicked() {
    app->showWindow(frame, 0);
    int floor_id;
    int choosen_slot = AssignParkingSlot(floor_id);  // if the function succeed, it will assign a value to the floor_id as well

    if (choosen_slot != -1)
    {
      db_parkingLot->floors[floor_id].slots[choosen_slot].is_taken = true;
      db_parkingLot->floors[floor_id].slots[choosen_slot].is_changed = true;

      app->quit(app->exitCode);
    }

    // Serial.begin(115200);
    // string FormatLine = formatString(FloorArr[choosen_slot].floor_id,choosen_slot);
    //mySerial.printf("data from vga to esp * %s *", FormatLine);
    //  mySerial.printf("data from vga to esp * %s *", choosen_slot);
  }


  /*String formatString(int floor_id, int i) 
   {
  // Create a string in the desired format
    String formattedString = "F" + String(floor_id) + "P" + String(i);
    return formattedString;
    }*/

  int AssignParkingSlot(int& floor_id) {
    int curr_parking = -1;

    for (int i = 0; i < num_of_floors; i++) {
      if (FloorArr[i].floor_frame != nullptr) {
        curr_parking = (FloorArr[i].floor_frame)->IfCheckedSetInTakenGroup();
      }

      if (FloorArr[i].floor_frame != nullptr && curr_parking >= 0) {
        floor_id = i;
        break;
      }
    }

    return curr_parking;
  }

  void uncheckParkingSlots() {
    for (int i = 0; i < num_of_floors; i++) {
      if (current_floor_id != i) {
        if ((FloorArr[i].floor_frame) != nullptr) {
          FloorArr[i].floor_frame->uncheckParkingSlots();
        }
      }
    }
  }

  void onFloorCheckboxClick(int floor_id) {
    // if this is the first time we are showing this floor, initiate it.
    if (FloorArr[floor_id].floor_frame == nullptr) {
      // set callback function to be called when a park slot has been clicked
      auto func = [&, this]() {
        this->uncheckParkingSlots();
      };

      // create a new floor fram and send the CB function to it
      FloorArr[floor_id].floor_frame = new FloorFrame(frame, ResX, ResY, app, floor_id, func);
    }

    // hide the current floor and show the new floor
    if (current_floor_id != floor_id) {
      if (current_floor_id >= 0 && FloorArr[current_floor_id].floor_frame != nullptr) {
        FloorArr[current_floor_id].floor_frame->hideFrame();
      }

      FloorArr[floor_id].floor_frame->showFrame();
      current_floor_id = floor_id;
    }
  }

  void freeResources()
  {
    for (int i = 0; i < num_of_floors; i++)
    {
      if(FloorArr[i].floor_frame != nullptr)
      {
        for (int j = 0; j < FloorArr[i].floor_frame->park_slots_num; j++)
        {
          Serial.println("delete FloorArr[" + String(i) + "].floor_frame->park_slots[" + String(j) + "]");
          delete FloorArr[i].floor_frame->park_slots[j];
        }

        Serial.println("free FloorArr[" + String(i) + "].floor_frame->park_slots");
        free(FloorArr[i].floor_frame->park_slots);

        Serial.println("delete FloorArr[" + String(i) + "].floor_frame");
        delete FloorArr[i].floor_frame;
      }
    }

    Serial.println("free FloorArr");
    free(FloorArr);
  }
};

//#endif // PARKINGLOTFRAME_H