#ifndef PARKSLOT_H
#define PARKSLOT_H

#include "fabgl.h"

class ParkSlot {

public:

    enum ParkStates {
        FREE,
        WAITING_FOR_VEHICLE,
        TAKEN
    };

    ParkSlot(fabgl::Canvas* cvToSet, int x, int y, int width, int height)
        : state(FREE), canvas(cvToSet), x_pos(x), y_pos(y), width(width), height(height) {

    }

    void draw()
    {
        // Paint a yellow rectangle
        canvas->setPenColor(Color::BrightWhite);
        canvas->drawRectangle(x_pos, y_pos, x_pos + width, y_pos + height);
    }

    void setPos(int x, int y)
    {
      x_pos = x;
      y_pos = y;
    }

private:

    ParkStates state;
    fabgl::Canvas* canvas;

    int width;
    int height;
    int x_pos;
    int y_pos;

};

#endif // PARKSLOT_H