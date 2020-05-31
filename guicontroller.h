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
#include "gtable.h"



using namespace std;

class GUIcontroller
{
public:
    /*
     * Constructor: GUIcontroller
     * Usage: GUIcontroller(string user)
     * -------------------
     * Initialize the GUIcontroller with specified current user.
     */
    GUIcontroller(string user);

    /*
     * Destructor: ~GUIcontroller
     * Usage: GUIcontroller()
     * -------------------
     * Delete the object in the memory when exit.
     */
    ~GUIcontroller();
    /*
     * Method:update
     * Usage:update()
     * --------------------------------------
     * This function is invoked to keep the GUI on running.
     */
    void update();
    /*
     * Method:isStop
     * Usage:isStop()
     * --------------------------------------
     * This function is invoked to check whether the GUI stops running.
     */
    bool isStop();

private:
    /*
     * Method:run
     * Usage:run(GWindow * gw)
     * --------------------------------------
     * This function is invoked when the GUI is running. It will catch the interactor's
     * action and based on their actions to refresh the GUI content.
     */
    void run(GWindow * gw);
    /*
     * Method:initThread
     * Usage:initThread()
     * --------------------------------------
     * This function is invoked to initialize the GUI of the Thread part. Moreover, the initialization
     * of the thread is also completed. The thread will also be added to simulated memory.
     */
    void initThread();
    void threadProcess();
    /*
     * Method:initFileSystem
     * Usage:initFileSystem()
     * --------------------------------------
     * This function is invoked to initialize the GUI of the File System. Moreover, the initialization
     * of the FS is also completed. The process of initialization of FS will also
     * be added to simulated memory.
     */
    void initFileSystem();
    void fileSystemProcess();
    /*
     * Method:initMain
     * Usage:initMain()
     * --------------------------------------
     * This function is invoked to initialize the layout of the GUI. Button will also be added.
     */
    void initMain();
    void mainWindowProcess();
    /*
     * Method:initMemory
     * Usage:initMemory()
     * --------------------------------------
     * This function is invoked to initialize the layout of the memory part of the GUI.
     */
    void initMemory();
    /*
     * Method:MemoryProcess
     * Usage:MemoryProcess()
     * --------------------------------------
     * This function is invoked to refresh the GUI of the memory part and show the latest
     * result of the memory.
     */
    void MemoryProcess();
    /*
     * Method:_widget
     * Usage:_widget()
     * --------------------------------------
     * This function is used to set some basic parameter such as title, size of the GUI.
     */
    void _widget();
    void initCalendar(GWindow * gw);
    void initCalculator(GWindow * gw);
    /*
     * Method:init_login
     * Usage:init_login()
     * --------------------------------------
     * This function is invoked to initialize the layout of the login part of the GUI.
     */
    void init_login();
    void user_login();
    /*
     * Method:dealFileSystem
     * Usage:dealFileSystem()
     * --------------------------------------
     * This function is invoked after the file system is initialized. This function is invoked to
     * handle file system functions such as touch, move, copy and so on. The result will also be
     * shown on the GUI.
     */
    void dealFileSystem();
    /*
     * Method:runCalculator
     * Usage:runCalculator()
     * --------------------------------------
     * This function is invoked when the calculator is running. It will invoke the calculator's
     * functions and also renew the GUI based on the result.
     */
    void runCalculator();
    /*
     * Method:runCalendar
     * Usage:runCalendar()
     * --------------------------------------
     * This function is invoked when the calendar is running. It will invoke the calendar's
     * functions and also renew the GUI based on the result.
     */
    void runCalendar();
    /*
     * Method:MemoryAdd
     * Usage:MemoryAdd(string APP_Name, string Process_Name, int Process_Index, int Memory_Declared)
     * --------------------------------------
     * This function is invoked to create and add new process into the OS memory.
     */
    void MemoryAdd(string APP_Name, string Process_Name, int Process_Index, int Memory_Declared);
    /*
     * Method:MemoryQuit
     * Usage:MemoryAdd(string APP_Name)
     * --------------------------------------
     * This function is invoked when we shut down an APP. The function will clear all the processes
     * belong to this APP.
     */
    void MemoryQuit(string APP_Name);
    /*
     * Method:set_reporter
     * Usage:set_reporter(GTextArea * reporter, string toadd, string info)
     * --------------------------------------
     * This function is invoked print error information on the reporter area of the GUI.
     */
    string set_reporter(GTextArea * reporter, string toadd, string info);


    bool runnable;
    GWindow * gw;
    int X;
    int Y;

    int user_login_x;
    int user_login_y;
    int user_login_w;
    int user_login_h;

    int calc_process_index = 0;
    int cala_process_index = 0;

    GButton * fileb;
    GButton * memoryb;
    GButton * calculatorb;
    GButton * calendarb;
    GButton * exitb;

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
    GTextArea * reporter_error;
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
    //Memory used size
    GContainer * memory_lay_top4;
    GLabel *memory_content2;
    // VM invoked Yes or No
    GContainer * memory_lay_top6;
    GLabel *memory_content3;
    // Memory Process Table
    GContainer * memory_table;
    GTable *mt;
    GTable *mt2;
    GContainer *memory_button;
    GChooser *SelectButton;
    GButton *OK;
    // Memory Bar
    Vector<GRect*> APP_Bars;
    GRect *APP_bar;

};

#endif // GUICONTROLLER_H
