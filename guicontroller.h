#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include "gwindow.h"
#include <string>
#include "spthread.h"
#include "glabel.h"
#include "gchooser.h"

using namespace std;

class GUIcontroller
{
public:
    GUIcontroller(string user);
    ~GUIcontroller();
    void update();
    bool isStop();

private:
    void run(GWindow * gw);
    void initThread();
    void threadProcess();
    void initFileSystem();
    void fileSystemProcess();
    void initMain();
    void mainWindowProcess();
    void initMemory();
    void MemoryProcess();
    void _widget();
    void initCalendar(GWindow * gw);
    void initCalculator(GWindow * gw);
    void init_login();
    void user_login();


    bool runnable;
    GWindow * gw;
    int X;
    int Y;

    int user_login_x;
    int user_login_y;
    int user_login_w;
    int user_login_h;
    string current_user;
    spthread * thread;
    GLabel * thread_log;
    GLabel * thread_user;
    GChooser * pick_user;


};

#endif // GUICONTROLLER_H
