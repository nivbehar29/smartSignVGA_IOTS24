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

  int calcWidthOfText(fabgl::FontInfo const * fontInfo, char const * text)
  {
    return app->canvas()->textExtent(fontInfo, text);
  }
};

#endif // GENERALFRAME_H