#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include "gwindow.h"

class GUIcontroller
{
public:
    GUIcontroller();
    ~GUIcontroller();
    void run();
    bool isStop();

private:
    bool runnable;
    GWindow gw;
};

#endif // GUICONTROLLER_H
