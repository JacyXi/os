#include "guicontroller.h"
#include "sFileSystem.h"
#include "gwindow.h"
#include "gbutton.h"
#include "gtextfield.h"
#include "gevent.h"


GUIcontroller::GUIcontroller()
{
    runnable = true;

}
GUIcontroller::~GUIcontroller(){
    runnable = false;
}

void GUIcontroller::run(){
    GButton *b = new GButton;
    gw.addToRegion(b,"SOUTH");
    gw.pause(10000);
}

bool GUIcontroller::isStop(){
    return !runnable;
}
