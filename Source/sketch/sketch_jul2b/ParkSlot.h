#ifndef PARKSLOT_H
#define PARKSLOT_H

#include "fabgl.h"
#include "GeneralFrame.h"

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
    uiButton* ChooseButton;

    int width;
    int height;
    int x_pos;
    int y_pos;

    ParkSlot(fabgl::uiFrame* frameToSet, fabgl::Canvas* cvToSet, int x, int y, int width, int height)
        : state(FREE), frame(frameToSet), canvas(cvToSet), x_pos(x), y_pos(y), width(width), height(height) {

      // Choose Button
      int ChooseButtonExt = calcWidthOfText(&fabgl::FONT_std_14, "Choose");
      Size ChooseButtonSize(ChooseButtonExt + 10, 20);
      ChooseButton = new uiButton(frame, "Choose", Point(x + width/2 - ChooseButtonSize.width/2, y + height - ChooseButtonSize.height - 5), ChooseButtonSize);
      //ChooseButton = new uiButton(frame, "Choose", Point(30, 30), ChooseButtonSize);
      ChooseButton->onClick = [&]() { onChooseButtonClick(); };

    }

    void draw()
    {
        // Paint a yellow rectangle
        int pen_width = 1;
        canvas->setPenWidth(pen_width);
        canvas->setPenColor(Color::White);
        canvas->drawRectangle(x_pos, y_pos, x_pos + width, y_pos + height);
        
        if(state == TAKEN)
        {
          canvas->setBrushColor(Color::Red);
          canvas->fillRectangle(x_pos + pen_width, y_pos + pen_width, x_pos + width - pen_width, y_pos + height - pen_width);
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
      if(state == FREE)
      {
        state = TAKEN;
        frame->app()->showWindow(ChooseButton, 0);
        
        frame->repaint();
      }

    }

};

#endif // PARKSLOT_H