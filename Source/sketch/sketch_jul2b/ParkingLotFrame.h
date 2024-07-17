//#ifndef PARKINGLOTFRAME_H
//#define PARKINGLOTFRAME_H

#pragma once
#include "fabgl.h"
#include "FloorFrame.h"
#include "string.h"
//#include "ParkSlot.h"


struct parking_floor
{
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
  
  ParkingLotFrame(uiFrame * parent_t, int ResX_t, int ResY_t, uiApp* app_t,int FloorsNum )
  {
    
    num_of_floors = FloorsNum;
    parent = parent_t;
    ResX = ResX_t;
    ResY = ResY_t;
    app = app_t;
    frame = new uiFrame(parent, "Parking Lot Frame", Point(0, 0), Size(ResX, ResY), false);

    FloorArr = (parking_floor*)malloc(num_of_floors * sizeof(parking_floor));

    current_floor_id = 0;

    // Frame properties
    fabgl::uiFrameProps& props = frame->frameProps();
    props.hasCloseButton = 0;
    props.hasMinimizeButton = 0;
    props.hasMaximizeButton = 0;
    props.moveable = 0;
    props.resizeable = 0;

    // Cancel Button
    int EndTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Finish");
    Size EndButtonSize(EndTextExt+ 20, 20);
    int cancel_y_top_offset = 10;
    int cancel_x_left_offset = 10;
    uiButton* EndButton = new uiButton(frame, "Finish", Point(cancel_x_left_offset, ResY - EndButtonSize.height - cancel_y_top_offset), EndButtonSize);
    EndButton->onClick = [&]() { 
                                  app->showWindow(frame, 0);
                                  AssignParkingSlot();
                               };




   
                             
    for(int i = 0; i < num_of_floors; i++)
    {
        FloorArr[i].num_of_regular_spaces=10;
        FloorArr[i].num_of_electric_spaces=5;
        FloorArr[i].num_of_disabled_spaces=2;
        FloorArr[i].num_of_motor_spaces=2;
        sprintf(FloorArr[i].floor_text, "Floor %d", i);

        int offset_y = 30;
        FloorArr[i].floor_label= new uiStaticLabel(frame, FloorArr[i].floor_text, Point(10, i*40 + offset_y));
        FloorArr[i].floor_box= new uiCheckBox(frame, Point(45, i*40 + offset_y), Size(16, 16), uiCheckBoxKind::RadioButton);
        FloorArr[i].floor_box->setGroupIndex(1);
        FloorArr[i].floor_frame = nullptr;

        FloorArr[i].floor_box->onClick = [&, i]() { onFloorCheckboxClick(i); };
    }

    // Activate first floor by default
    FloorArr[0].floor_box->onClick();
  }
    void AssignParkingSlot()
    {
      for(int i = 0; i < num_of_floors; i++) 
      {
      
        if((FloorArr[i].floor_frame)->IfChecked())
        {
            break;
        }
      
      }

    }

   void uncheckParkingSlots() {
    for(int i = 0; i < num_of_floors; i++) {
      if(current_floor_id != i)
      {
        if((FloorArr[i].floor_frame)!=nullptr)
        {
             FloorArr[i].floor_frame->uncheckParkingSlots();
        }
        
      }
    }
   }
  void onFloorCheckboxClick(int floor_id)
  {
    // if this is the first time we are showing this floor, initiate it.
    if(FloorArr[floor_id].floor_frame == nullptr)
    {
      auto func=[&,this](){this->uncheckParkingSlots();}; 
      FloorArr[floor_id].floor_frame = new FloorFrame(frame, ResX, ResY, app, floor_id,func);
    }

    // hide the current floor and show the new floor
    if(current_floor_id != floor_id)
    {
      FloorArr[current_floor_id].floor_frame->hideFrame();
      FloorArr[floor_id].floor_frame->showFrame();
      current_floor_id = floor_id;
    }


  }


  




};

//#endif // PARKINGLOTFRAME_H