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
#include "gslider.h"
#include "math.h"
#include <iostream>


using namespace std;


GUIcontroller::GUIcontroller(string user)
{
    runnable = true;
    current_user = user;
    gw = new GWindow(getScreenWidth(),getScreenHeight());

    _widget();

    initThread();
    initMemory();
    initMain();
    initFileSystem();

    run(gw);




}
GUIcontroller::~GUIcontroller() {
    runnable = false;
    gw->close();
}

void GUIcontroller::run(GWindow * gw) {

     while (true) {
        GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
        if (e.getEventType() == MOUSE_CLICKED) break;
        cout << "Please do not press this button again." << endl;
     }


}

void GUIcontroller::_widget(){
    X = gw->getWidth();
    Y = gw->getHeight();
    gw->setResizable(true);
    GImage * back_ground = new GImage("background.jpg");
    back_ground->sendToBack();
    gw->add(back_ground);
    gw->setTitle("Simulated Operating System");

}
void GUIcontroller::initThread() {
    int line_space = Y/50;
    int x1 = X/4;
    int y1 = 4*Y/5+2*line_space;

    thread = new spthread(current_user, true, false);
    string information = "Initialize a thread for user : ";
    information.append(current_user);

    GContainer * con = new GContainer;
    GSlider * sl = new GSlider(0,100,50);
    sl->setBounds(x1,y1+10,100,50);
    con->add(sl);
    con->setBounds(x1,y1,x1,y1);
    con->setX(100);
    con->setY(100);
    pick_user = new GChooser();
    pick_user->setBounds(x1,y1,100,10);
    pick_user->addItem("Jacy");
    pick_user->addItem("Eric");
    pick_user->addItem("Blaine");
    pick_user->addItem("Chenpi");
    //gw->add(pick_user,10,10);
    pick_user->setName("AAA");
    con->add(pick_user);
}

void GUIcontroller::initFileSystem() {

}

void GUIcontroller::initMain(){
    GImage * table = new GImage("main.png");
    table->setWidth(3*X/5);
    table->setHeight(23*Y/30);
    gw->add(table,X/5,0);

}

void GUIcontroller::init_login() {
    user_login_x = 0;
    user_login_y = 3.84/5*Y;
    user_login_w = X/5;
    user_login_h = Y/30;


}

void GUIcontroller::initMemory() {

}

void GUIcontroller::update() {
    run(gw);

}

bool GUIcontroller::isStop() {
    return !runnable;
}
