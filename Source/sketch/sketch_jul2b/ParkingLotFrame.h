#pragma once

#include "fabgl.h"
#include "FloorFrame.h"
#include "string.h"
#include "DBAux.h"

extern DB_parkingLot* db_parkingLot;

struct parking_floor {
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
  int selectedParkingType;

  uiMessageBoxResult* msg = nullptr;
  uiTimerHandle msgTimer;


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
    frame = new uiFrame(parent, "", Point(0, 0), Size(ResX, ResY), false);

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
    Size FinishButtonSize(50, 40);
    int finish_y_top_offset = 70;
    int finish_x_left_offset = 10;
    uiButton* FinishButton = new uiButton(frame, "Finish", Point(finish_x_left_offset, ResY - FinishButtonSize.height - finish_y_top_offset), FinishButtonSize);
    FinishButton->buttonStyle().textColor=RGB888(255,255,255);
    //FinishButton->buttonStyle().textFont = &fabgl::FONT_std_16;
    FinishButton->buttonStyle().backgroundColor = RGB888(0,153,153);
    FinishButton->onClick = [&]() {
      onFinishButtonClicked();
    };

    // Cancel Button
    int CancelTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Cancel");
    Size CancelButtonSize(50, 40);
    int cancel_y_top_offset = 10;
    int cancel_x_left_offset = 10;
    uiButton* CancelButton = new uiButton(frame, "Cancel", Point(cancel_x_left_offset, ResY - CancelButtonSize.height - cancel_y_top_offset - FinishButtonSize.height - finish_y_top_offset), CancelButtonSize);
    CancelButton->onClick = [&]() {
      onCancelButtonClicked();
    };

    //
    for (int i = 0; i < num_of_floors; i++)
    {
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
  }

  void showFrame()
  {
    app->showWindow(frame, 1);
    app->setActiveWindow(frame);

    FloorArr[0].floor_box->setChecked(true);
    onFloorCheckboxClick(0);
    
  }

  void onFinishButtonClicked() {
    
    int floor_id;
    int choosen_slot = AssignParkingSlot(floor_id);  // if the function succeed, it will assign a value to the floor_id and return the parking slot id , else - return -1

    if(choosen_slot != -1 && current_floor_id != -1)
    {
      app->showWindow(frame, 0);
      Serial.println("Calling freeFloorArrSlot()");
      freeFloorArrSlot(current_floor_id);
      current_floor_id = -1;

      if(db_parkingLot != nullptr)
      {
        db_parkingLot->floors[floor_id].slots[choosen_slot].is_taken = true;
        db_parkingLot->floors[floor_id].slots[choosen_slot].is_changed = true;

        // Show a massage to the customer
        String choosen_slot_str =  String(choosen_slot);
        if(choosen_slot_str.length() == 1)
          choosen_slot_str = String(0) + choosen_slot_str;
        String msg_str = "Your parking code is: " + String(floor_id) + choosen_slot_str + ". Please save it.";
        Serial.println(msg_str);
        app->messageBox(nullptr, msg_str.c_str(), "Ok", nullptr, nullptr, uiMessageBoxIcon::Info); 

        app->quit(app->exitCode);
      }
    }
    else
    {
      // Show a massage to the customer
      Serial.println("Please select a slot");
      app->messageBox(nullptr, "Please select a slot", "Ok", nullptr, nullptr, uiMessageBoxIcon::Error);                                          
    }
  }

  void onCancelButtonClicked()
  {
    app->showWindow(frame, 0);
    if(current_floor_id != -1)
    {
      freeFloorArrSlot(current_floor_id);
      current_floor_id = -1;
    }
  }

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

    if(floor_id != current_floor_id)
    {
      int last_floor_id = current_floor_id;

      // Hide and free the current floor
      if (current_floor_id != floor_id) {
        if (current_floor_id >= 0 && FloorArr[current_floor_id].floor_frame != nullptr) {
          // Hide current floor
          FloorArr[current_floor_id].floor_frame->hideFrame();

          // Delete last floor frame to save memory
          freeFloorArrSlot(last_floor_id);
        }
      }

      // Create and show the new floor
      if (FloorArr[floor_id].floor_frame == nullptr) {
        // set callback function to be called when a park slot has been clicked
        auto func = [&, this]() {
          this->uncheckParkingSlots();
        };

        // create a new floor frame and send the CB function to it
        FloorArr[floor_id].floor_frame = new FloorFrame(frame, ResX, ResY, app, floor_id, func);

        // Set selected parking type to the new floor
        FloorArr[floor_id].floor_frame->setSelectedParkingType(selectedParkingType);

        // Show selected floor
        FloorArr[floor_id].floor_frame->showFrame();
        current_floor_id = floor_id;
      }
    }
  }

  void setSelectedParkingType(int type)
  {
    selectedParkingType = type;
  }

  void freeFloorArrSlot(int i)
  {
    if(FloorArr[i].floor_frame != nullptr)
    {
      for (int j = 0; j < FloorArr[i].floor_frame->park_slots_num; j++)
      {
        delete FloorArr[i].floor_frame->park_slots[j];
      }

      free(FloorArr[i].floor_frame->park_slots);

      app->destroyWindow(FloorArr[i].floor_frame->frame);
      delete FloorArr[i].floor_frame;
      FloorArr[i].floor_frame = nullptr;
    }
  }

  void freeResources()
  {
    for (int i = 0; i < num_of_floors; i++)
    {
      freeFloorArrSlot(i);
    }

    Serial.println("free FloorArr");
    free(FloorArr);
  }
};