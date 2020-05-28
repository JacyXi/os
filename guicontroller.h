#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include "gwindow.h"
#include <string>
#include "spthread.h"
#include "glabel.h"
#include "gchooser.h"
#include "gbutton.h"
#include "gtextarea.h"
#include "sFileSystem.h"
#include "gtextfield.h"
#include "memory.h"
#include "map.h"

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
    void dealFileSystem();
    void runCalculator();
    void runCalendar();


    string set_reporter(GTextArea * reporter, string toadd, string info);
    bool runnable;
    GWindow * gw;
    int X;
    int Y;

    int user_login_x;
    int user_login_y;
    int user_login_w;
    int user_login_h;

    GButton * fileb;
    GButton * memoryb;
    GButton * calculatorb;
    GButton * calendarb;

    string current_user;
    spthread * thread;
    GLabel * thread_log;
    GLabel * thread_user;
    GChooser * pick_user;
    GButton * write;
    GButton * read;
    GButton * unlock;

    GButton * confirm_user;
    GTextField * input_name;
    GTextField * input_word;

    string wrinfo;
    int rows;
    GTextArea * reporter_wrlock;
    GTextArea * reporter_pool;
    string poolinfo;
    sFileSystem * fs;
    GLabel * pwd;
    GTextArea * file_info;
    GTextArea * ls;
    GTextField * input_file;
    GChooser * input_oper;
    GButton * oper;
    GImage * table;
    Map<string,int> command;
    Map<string, string> login;
    Memory * mo;





};

#endif // GUICONTROLLER_H
