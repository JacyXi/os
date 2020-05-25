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
#include "gtextarea.h"


using namespace std;


GUIcontroller::GUIcontroller(string user)
{
    runnable = true;
    current_user = user;
    gw = new GWindow(getScreenWidth(),getScreenHeight());

    _widget();

    initThread();
    //initMemory();
    initMain();
    init_login();
    //initFileSystem();

    run(gw);


}
GUIcontroller::~GUIcontroller() {
    runnable = false;
    gw->close();
}

void GUIcontroller::run(GWindow * gw) {

     while (true) {
        GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
        if (e.getEventType() == MOUSE_CLICKED) {
            e.getX();
            e.getY();

            break;
        }
        cout << "Please do not press this button again." << endl;
     }



}

void GUIcontroller::_widget(){
    X = gw->getWidth();
    Y = gw->getHeight();
    gw->setResizable(true);
    GImage * back_ground = new GImage("background.png");
    back_ground->sendToBack();
    gw->add(back_ground);
    gw->setTitle("Simulated Operating System");

}

void GUIcontroller::initThread() {


    //log in
    GContainer * thread_lay0 = new GContainer;
    thread_lay0->setX(1.3*X/200);
    thread_lay0->setY(Y/5*4.03);
    thread_lay0->setWidth(0.98*X/5);
    thread_lay0->setHeight(Y/18);
    GLabel *name = new GLabel("name:");
    GTextField * input_name = new GTextField();
    GLabel *password = new GLabel("password:");
    GTextField * input_word = new GTextField();



    thread_lay0->add(name);
    thread_lay0->add(input_name);
    thread_lay0->add(password);
    thread_lay0->add(input_word);

    GContainer * thread_lay01 = new GContainer;
    thread_lay01->setX(1.3*X/200);
    thread_lay01->setY(Y/5*4.2);
    thread_lay01->setWidth(0.98*X/5);
    thread_lay01->setHeight(1.4*Y/18);
    confirm_user = new GButton("Confirm");
    thread_lay01->add(confirm_user);

    thread_lay0->add(name);
    thread_lay0->add(input_name);
    thread_lay0->add(password);
    thread_lay0->add(input_word);

    //w r lock
    GContainer * thread_lay1 = new GContainer;
    thread_lay1->setBounds(1.3*X/200,Y*0.919,0.98*X/5,1*Y/18);
    pick_user = new GChooser();
    pick_user->setBounds(X/144,Y,1.3*X/20,Y/20);
    pick_user->addItem("Jacy");

    write = new GButton("write");
    read = new GButton("read");

    thread_lay1->add(pick_user);
    thread_lay1->add(write);
    thread_lay1->add(read);




    //r-w-reporter
    GContainer * thread_lay2 = new GContainer();
    GTextArea * reporter_wrlock = new GTextArea();
    thread_lay2->setBounds(0.208*X,Y/5*3.99,X*0.287,1.07*Y/6);
    reporter_wrlock->setRows(10);
    reporter_wrlock->setEditable(false);
    reporter_wrlock->setWidth(X*0.284);
    wrinfo = "Initialize the reader-writer lock reporter.\n";
    reporter_wrlock->setText(wrinfo);
    thread_lay2->add(reporter_wrlock);


    //thread pool reporter
    GContainer * thread_lay3 = new GContainer();
    thread_lay3->setBounds(X/2,Y/2,100,70);
    thread_lay3->setBackground("Black");
    GLabel * l = new  GLabel("aaaa");
    l->setColor("White");
    thread_lay3->add(l);



    thread = new spthread(current_user, true, false);
    string information = "Initialize a thread for user : ";
    information.append(current_user);

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
