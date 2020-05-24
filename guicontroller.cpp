#include "guicontroller.h"
#include "sFileSystem.h"
#include "gwindow.h"
#include "gbutton.h"
#include "gtextfield.h"
#include "gevent.h"
#include "glabel.h"
#include "gchooser.h"
#include "spthread.h"
#include <string>
#include "QMenu"
#include "gobjects.h"

using namespace std;


GUIcontroller::GUIcontroller(string user)
{
    runnable = true;
    current_user = user;
    gw = new GWindow(getScreenWidth(),getScreenHeight());

    _widget();
    run(gw);
    initThread();
    initMemory();
    initMain();
    initFileSystem();



}
GUIcontroller::~GUIcontroller(){
    runnable = false;
}

void GUIcontroller::run(GWindow * gw){

}

void GUIcontroller::_widget(){
    X = gw->getWidth();
    Y = gw->getHeight();
    gw->setWindowTitle("Simulated Operating System");
    gw->setColor(123);
    gw->drawLine(0, 4*Y / 5, X, 4*Y/5);
    gw->drawLine(0,Y/30,X,Y/30);
    gw->drawLine(0,3*Y/30, X/5,3*Y/30);
    gw->drawLine(0,13*Y/30,X/5,13*Y/30);
    gw->drawLine(X/5,Y/30,X/5,4*Y/5);
    gw->drawLine(X/4,Y,X/4,4*Y/5);
    gw->drawLine(3*X/4,Y,3*X/4,4*Y/5);
    gw->drawLine(4*X/5,Y/30,4*X/5,4*Y/5);
    gw->drawLine(4*X/5,13*Y/30,X,13*Y/30);

}
void GUIcontroller::initThread() {
    int line_space = Y/50;
    int x1 = X/4;
    int y1 = 4*Y/5+2*line_space;

    thread = new spthread(current_user, true, false);
    string information = "Initialize a thread for user : ";
    information.append(current_user);
    thread_log = new GLabel("Thread log");
    thread_log->setFont("SansSerif-20");
    gw->add(thread_log, x1+X/5, y1);
    thread_user = new GLabel("W-R-lock user");
    thread_user->setFont("SansSerif-20");
    gw->add(thread_user, X/12, y1);
    pick_user = new GChooser();
    pick_user->setBounds(x1,y1,100,10);
    pick_user->addItem("Jacy");
    pick_user->addItem("Eric");
    pick_user->addItem("Blaine");
    pick_user->addItem("Chenpi");
    gw->add(pick_user,10,10);
    pick_user->setName("AAA");
}

void GUIcontroller::initFileSystem(){


}
void GUIcontroller::initMain(){

}

void GUIcontroller::initMemory(){

}

void GUIcontroller::update(){


}

bool GUIcontroller::isStop(){
    return !runnable;
}
