#ifndef PARKINGLOTFRAME_H
#define PARKINGLOTFRAME_H

#include "fabgl.h"

  struct parking_floor
  {
    int num_of_regular_spaces;
    int num_of_disabled_spaces;
    int num_of_electric_spaces;
    int num_of_motor_spaces;
    uiStaticLabel* floor_label;
    uiCheckBox* floor_box;

  };

class ParkingLotFrame : public GeneralFrame {

 //struct parking_floor* FloorArr;

  public:
  
  int num_of_floors;
  parking_floor* FloorArr;
  
ParkingLotFrame(uiFrame * parent_t, int ResX_t, int ResY_t, uiApp* app_t,int FloorsNum )
{
          num_of_floors=FloorsNum;
          parent = parent_t;
          ResX = ResX_t;
          ResY = ResY_t;
           app = app_t;
          frame = new uiFrame(parent, "Options Frame", Point(0, 0), Size(ResX, ResY), false);
          FloorArr = (parking_floor*)malloc(num_of_floors* sizeof(parking_floor));

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
          CancelButton->onClick = [&]() { app->showWindow(frame, 0);
                                        };
                                        
          for(int i=0;i<num_of_floors;i++)
          {
              FloorArr[i].num_of_regular_spaces=10;
              FloorArr[i].num_of_electric_spaces=5;
              FloorArr[i].num_of_disabled_spaces=2;
              FloorArr[i].num_of_motor_spaces=2;
              FloorArr[i].floor_label= new uiStaticLabel(frame, "Floor", Point(10, i*40));
              FloorArr[i].floor_box= new uiCheckBox(frame, Point(45, i*40), Size(16, 16), uiCheckBoxKind::RadioButton);
              FloorArr[i].floor_box->setGroupIndex(1);

          }
                             



}


};

#endif // PARKINGLOTFRAME_H