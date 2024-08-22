#ifndef EXITFRAME_H
#define EXITFRAME_H

#include "fabgl.h"
#include "fabui.h"
#include "GeneralFrame.h"
#include "Bitmaps.h"

class ExitFrame : public GeneralFrame {

  public:

  int buttonSizeX = 50;
  int buttonSizeY = 50;

  uiLabel* CodeText;
  String CurrentCodeText;
  bool IsCodeTextInit = false;
  String CodeTextDefault = "Please Enter Code";

  ExitFrame(uiFrame * parent_t, int ResX_t, int ResY_t, uiApp* app_t )
  {
    parent = parent_t;
    ResX = ResX_t;
    ResY = ResY_t;
    app = app_t;

    int codeTextDefaultExt = calcWidthOfText(&fabgl::FONT_std_24, CodeTextDefault.c_str());
    int windowSizeX = max(codeTextDefaultExt, buttonSizeX * 3) + 20;
    int windowSizeY = buttonSizeY * 6;
    frame = new uiFrame(parent, nullptr, Point(ResX / 2 - windowSizeX / 2, ResY / 2 - windowSizeY / 2), Size(windowSizeX, windowSizeY), false);

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
    uiButton* CancelButton = new uiButton(frame, "Cancel", Point(cancel_x_left_offset, windowSizeY - CancelButtonSize.height - cancel_y_top_offset), CancelButtonSize);
    CancelButton->onClick = [&]() { onCancelButtonClick();};

    // Finish Button
    int FinishTextExt = calcWidthOfText(&fabgl::FONT_std_14, "Finish");
    Size FinishButtonSize(FinishTextExt + 20, 20);
    int finish_y_top_offset = 10;
    int finish_x_left_offset = 10;
    uiButton* FinishButton = new uiButton(frame, "Finish", Point(windowSizeX - FinishButtonSize.width - finish_x_left_offset, windowSizeY - FinishButtonSize.height - finish_y_top_offset), FinishButtonSize);
    FinishButton->onClick = [&]() { onFinishButtonClick();};

    // Setup Buttons
    Size buttonsSize(buttonSizeX, buttonSizeY);
    int h = buttonSizeY;
    int w = buttonSizeX;

    int y_off = 10; // y offset for all buttons
    int x_off = (windowSizeX - 3 * w) / 2;

    uiButton* Button1 =   new uiButton(frame, "1", Point(x_off + 0,   y_off),       buttonsSize);
    uiButton* Button2 =   new uiButton(frame, "2", Point(x_off + w,   y_off),       buttonsSize);
    uiButton* Button3 =   new uiButton(frame, "3", Point(x_off + 2*w, y_off),       buttonsSize);
    uiButton* Button4 =   new uiButton(frame, "4", Point(x_off + 0,   y_off + h),   buttonsSize);
    uiButton* Button5 =   new uiButton(frame, "5", Point(x_off + w,   y_off + h),   buttonsSize);
    uiButton* Button6 =   new uiButton(frame, "6", Point(x_off + 2*w, y_off + h),   buttonsSize);
    uiButton* Button7 =   new uiButton(frame, "7", Point(x_off + 0,   y_off + 2*h), buttonsSize);
    uiButton* Button8 =   new uiButton(frame, "8", Point(x_off + w,   y_off + 2*h), buttonsSize);
    uiButton* Button9 =   new uiButton(frame, "9", Point(x_off + 2*w, y_off + 2*h), buttonsSize);
    uiButton* Button0 =   new uiButton(frame, "0", Point(x_off + w,   y_off + 3*h), buttonsSize);
    uiButton* ButtonDel = new uiButton(frame, "<", Point(x_off + 2*w, y_off + 3*h), buttonsSize);

    Button1->onClick = [&]() {onNumberButtonClick(1);};
    Button2->onClick = [&]() {onNumberButtonClick(2);};
    Button3->onClick = [&]() {onNumberButtonClick(3);};
    Button4->onClick = [&]() {onNumberButtonClick(4);};
    Button5->onClick = [&]() {onNumberButtonClick(5);};
    Button6->onClick = [&]() {onNumberButtonClick(6);};
    Button7->onClick = [&]() {onNumberButtonClick(7);};
    Button8->onClick = [&]() {onNumberButtonClick(8);};
    Button9->onClick = [&]() {onNumberButtonClick(9);};
    Button0->onClick = [&]() {onNumberButtonClick(0);};
    
    ButtonDel->onClick = [&]() {onDelButtonClick();};

    // Code Text
    CurrentCodeText = CodeTextDefault;
    int codeTextExt = calcWidthOfText(&fabgl::FONT_std_24, CurrentCodeText.c_str());
    CodeText = new uiLabel(frame, CurrentCodeText.c_str(), Point(windowSizeX / 2 - codeTextExt / 2, y_off + 4*h + 20));
    CodeText->labelStyle().backgroundColor = RGB888(255,255,255);
    CodeText->labelStyle().textFont        = &fabgl::FONT_std_24;
    CodeText->update();
  }

  void updateCodeText(String new_text)
  {
    int codeTextExt_prev = calcWidthOfText(&fabgl::FONT_std_24, CurrentCodeText.c_str());

    CurrentCodeText = new_text;
    CodeText->setText(new_text.c_str());
    CodeText->update();

    int codeTextExt = calcWidthOfText(&fabgl::FONT_std_24, new_text.c_str());
    int newX = CodeText->pos().X + codeTextExt_prev / 2 - codeTextExt / 2;
    int newY = CodeText->pos().Y;
    app->moveWindow(CodeText, newX, newY);
  }

  void onNumberButtonClick(int num)
  {
    String newText;

    if(!IsCodeTextInit)
    {
      newText = String(num);
      IsCodeTextInit = true;
    }
    else
    {
      if(CurrentCodeText.length() < 3)
      {
        newText = CurrentCodeText + String(num);
      }
      else
      {
        // Do nothing
        return;
      }
    }

    updateCodeText(newText);
  }

  void onDelButtonClick()
  {
    if(IsCodeTextInit && CurrentCodeText.length() >= 1)
    {
      String new_text = CurrentCodeText;

      // Remove the last character
      new_text.remove(CurrentCodeText.length() - 1);
      
      updateCodeText(new_text);
    }
  }

  void onCancelButtonClick()
  {
    updateCodeText(CodeTextDefault);
    IsCodeTextInit = false;

    app->showWindow(frame, 0);
  }

  void onFinishButtonClick()
  {
    if(IsCodeTextInit && CurrentCodeText.length() == 3)
    {
      if(db_parkingLot != nullptr)
      {
        // Get the first letter (floor)
        int floor_id = CurrentCodeText.substring(0, 1).toInt();

        // Get the rest of the string (parking slot)
        int choosen_slot = CurrentCodeText.substring(1).toInt();

        if( floor_id < db_parkingLot->num_floors && 
            choosen_slot < db_parkingLot->floors[floor_id].num_slots && 
            db_parkingLot->floors[floor_id].slots[choosen_slot].is_taken)
        {
          db_parkingLot->floors[floor_id].slots[choosen_slot].is_taken = false;
          db_parkingLot->floors[floor_id].slots[choosen_slot].is_changed = true;

          updateCodeText(CodeTextDefault);
          IsCodeTextInit = false;
          app->quit(app->exitCode);
        }
        else
        {
          // Show message about wrong code
          String msg_str = "Wrong code: " + CurrentCodeText;
          Serial.println(msg_str);
          app->messageBox(nullptr, msg_str.c_str(), "Ok", nullptr, nullptr, uiMessageBoxIcon::Info); 
        }
      }
    }
    else
    {
      // Show message about wrong code
      String msg_str = "Please enter a valid code";
      Serial.println(msg_str);
      app->messageBox(nullptr, msg_str.c_str(), "Ok", nullptr, nullptr, uiMessageBoxIcon::Info); 
    }
    
  }
};

#endif // EXITFRAME_H