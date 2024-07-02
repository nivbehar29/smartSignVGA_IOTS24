#ifndef PARKINGLOT_H
#define PARKINGLOT_H

#include "fabgl.h"
#include "ParkSlot.h"

class ParkingLot {

public:


    ParkingLot(fabgl::Canvas* cvToSet)
        : canvas(cvToSet) {

          int width = 100;
          int height = 150;
          int width_with_space = 10 + width;

          parkSlots[0] = new ParkSlot(cvToSet, 10, 10, width, height);
          parkSlots[1] = new ParkSlot(cvToSet, 10 + 1*width_with_space, 10, width, height);
          parkSlots[2] = new ParkSlot(cvToSet, 10 + 2*width_with_space, 10, width, height);
          parkSlots[3] = new ParkSlot(cvToSet, 10 + 3*width_with_space, 10, width, height);

    }

    void draw()
    {
        for(int i = 0; i < num_slots; i++)
        {
          parkSlots[i]->draw();
        }
    }

private:

    fabgl::Canvas* canvas;

    ParkSlot* parkSlots[4];
    int num_slots = 4;

};

#endif // PARKINGLOT_H