#ifndef BITMAP_IOT_H
#define BITMAP_IOT_H

#include "fabgl.h"

class Bitmap_iot {
private:
    fabgl::Bitmap bm_buff;
    int x;
    int y;

public:
    Bitmap_iot(fabgl::Bitmap *bitmap, int xpos, int ypos)
        : x(xpos), y(ypos) {
        if (bitmap != nullptr) {
            bm_buff = *bitmap;
        }
    }

    void setBitmap(fabgl::Bitmap *bitmap) {
        bm_buff = *bitmap;
    }

    void setPos(int xpos, int ypos) {
        x = xpos;
        y = ypos;
    }

    fabgl::Bitmap* getBitmap() {
        return &bm_buff;
    }

    struct Pos {
        int x;
        int y;
    };

    Pos getPos() {
        Pos pos;
        pos.x = x;
        pos.y = y;
        return pos;
    }
};

#endif // BITMAP_IOT_H