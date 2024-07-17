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

    std::function<void()> func;

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

    ParkSlot(fabgl::uiFrame* frameToSet, fabgl::Canvas* cvToSet, int x, int y, int width, int height,std::function<void()> funcToSet)
        : state(FREE), frame(frameToSet), canvas(cvToSet), x_pos(x), y_pos(y), width(width), height(height) {


       func=funcToSet;
       //curr_frame=curr_frameToSet;
     /*
      // Choose Button
      int ChooseButtonExt = calcWidthOfText(&fabgl::FONT_std_14, "Choose");
      Size ChooseButtonSize(ChooseButtonExt + 10, 20);
      ChooseButton = new uiButton(frame, "Choose", Point(x + width/2 - ChooseButtonSize.width/2, y + height - ChooseButtonSize.height - 5), ChooseButtonSize);
      //ChooseButton = new uiButton(frame, "Choose", Point(30, 30), ChooseButtonSize);
      ChooseButton->onClick = [&]() { onChooseButtonClick(); }; */
        // num_of_floors=num_of_floorsToSet;
        // Floorarr=Floorarray;
        // current_floor_id=curr_id;
        int offset_y = 30;
       int ChooseButtonExt = calcWidthOfText(&fabgl::FONT_std_14, "Choose");
       Size ChooseButtonSize(ChooseButtonExt + 10, 20);
       // uistaticLabel* ChooseLabel= new uiStaticLabel(frame, "choose", Point(10, 40 ));
        ChooseButton= new uiCheckBox(frame, Point(x + width/2 - ChooseButtonSize.width/2, y + height - ChooseButtonSize.height - 5), Size(50, 120), uiCheckBoxKind::RadioButton);
        
        ChooseButton->setGroupIndex(1);
        ChooseButton->checkBoxStyle().mouseOverBackgroundColor=RGB888(0,0,0);
        ChooseButton->checkBoxStyle().checkedBackgroundColor=RGB888(0,128,0);
        ChooseButton->checkBoxStyle().foregroundColor=RGB888(0,128,0);
        ChooseButton->repaint();
        ChooseButton->onClick = [&]() {onChooseButtonClick();};

        }

        void SetGroup()
        {
           ChooseButton->setGroupIndex(-1);
        }
        
        void uncheckNotTaken() 
        {
          if(ChooseButton->groupIndex()!=-1)
          {
              ChooseButton->setChecked(false);
          }
          
        }
        

    void draw()
    {
       /* // Paint a yellow rectangle
        int pen_width = 1;
        canvas->setPenWidth(pen_width);
        canvas->setPenColor(Color::White);
        canvas->drawRectangle(x_pos, y_pos, x_pos + width, y_pos + height);
        
        if(state == TAKEN)
        {
          canvas->setBrushColor(Color::Red);
          canvas->fillRectangle(x_pos + pen_width, y_pos + pen_width, x_pos + width - pen_width, y_pos + height - pen_width);
        }*/
        return;

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
      // for(int i = 0; i < num_of_floors; i++)
      //   {
      //   if (i != current_floor_id) 
      //   {
      //     int num_of_park_slots = Floorarr[i].floorframe->park_slots_num;
      //     for(int j = 0; j < num_of_park_slots; j++) 
      //     {
      //       Floorarr[i].floorFrame->parkslots[i].uncheck();
      //     }
      //   }
      //   }
            func();

         
    }

};

//#endif // PARKSLOT_H