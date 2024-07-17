#ifndef GENERALFRAME_H
#define GENERALFRAME_H

#include "fabgl.h"

class GeneralFrame {

  public:

  uiFrame* frame;
  uiFrame* parent;
  int ResX;
  int ResY;
  uiApp* app;
  //parking_floor* FloorArr;

  void showFrame()
  {
    app->showWindow(frame, 1);
  }

  void hideFrame()
  {
    app->showWindow(frame, 0);
  }

  int calcWidthOfText(fabgl::FontInfo const * fontInfo, char const * text)
  {
    return app->canvas()->textExtent(fontInfo, text);
  }
};

#endif // GENERALFRAME_H