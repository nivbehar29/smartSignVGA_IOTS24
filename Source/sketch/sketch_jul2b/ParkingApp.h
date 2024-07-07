#ifndef PARKINGAPP_H
#define PARKINGAPP_H

class ParkingApp : public uiApp {

    uiFrame* ControlFrame;
    uiButton* WelcomeButton;

    void init() {

      // set root window background color to dark green
      rootWindow()->frameStyle().backgroundColor = RGB888(0, 64, 0);

      // frame where to put buttons
      ControlFrame = new uiFrame(rootWindow(), "", Point(10, 10), Size(115, 120));
      ControlFrame->frameStyle().backgroundColor = RGB888(0, 0, 255);
      ControlFrame->windowStyle().borderSize     = 0;

      // Welcome Button
      WelcomeButton = new uiButton(ControlFrame, "Welcome Button !!!!!!!!!!!!!!!!!!!!!", Point(10, 10), Size(105, 20));
      WelcomeButton->onClick = [&]() { onWelcomeButtonClick(); };

    }

  void onWelcomeButtonClick() {
    // // show TestModalDialog as modal window
    // auto testModalDialog = new TestModalDialog(rootWindow());
    // showModalWindow(testModalDialog);
    // destroyWindow(testModalDialog);
  }
};

#endif // PARKINGAPP_H