#include "guicontroller.h"
#include "sFileSystem.h"
#include "Calculator.h"
#include "sCalendar.h"
#include "spthread.h"
#include "memory.h"

#include "gwindow.h"
#include "gobjects.h"
#include "gbutton.h"
#include "gtextfield.h"
#include "gtextarea.h"
#include "gevent.h"
#include "glabel.h"
#include "gchooser.h"
#include "gtable.h"

#include <iostream>
#include <string>
#include "math.h"
#include "set.h"
#include "error.h"

using namespace std;


GUIcontroller::GUIcontroller(string user)
{
    runnable = true;
    current_user = user;
    gw = new GWindow(getScreenWidth(),getScreenHeight());

    mo = new Memory();

    _widget();

    initThread();
    init_login();
    initMain();
    initMemory();
    run(gw);


}

GUIcontroller::~GUIcontroller() {
    runnable = false;
    gw->close();
}

void GUIcontroller::run(GWindow * gw) {
    string comd;
    while (true) {
       GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
       comd = e.getActionCommand();
       if (comd.length() > 0) break;
    }
    switch (command.get(comd))
    {
    case 0:{
        string user_name = input_name->getText();
        string user_password =  input_word->getText();
        if (!login.get(user_name).compare(user_password)) {

            current_user = user_name;
            pick_user->addItem(user_name);
            pick_user->setSelectedItem(user_name);
            wrinfo = set_reporter(reporter_wrlock, thread->init_lock(user_name), wrinfo);
        }
        break;
    }

    case 1:{
        string user = pick_user->getSelectedItem();
        wrinfo = set_reporter(reporter_wrlock, thread->wrlock(user), wrinfo);
        break;
    }

    case 2:{
        string user = pick_user->getSelectedItem();
        wrinfo = set_reporter(reporter_wrlock, thread->rdlock(user), wrinfo);
        break;
    }
    case 3:{
        string user = pick_user->getSelectedItem();
        wrinfo = set_reporter(reporter_wrlock, thread->unlock(user), wrinfo);
        break;
    }
    case 4:{
        dealFileSystem();
        break;
    }
    case 5:{
        initFileSystem();
        break;
    }
    case 6:{
        MemoryProcess();
        break;
    }
    case 7:{
        runCalculator();
        break;
    }
    case 8:{
        //calendar
        runCalendar();
        break;
    }
    case 9:{
        // OK Button
        MemoryProcess();
        break;
    }
    case 99:{
        runnable = false;
        break;
    }
    default:
        break;
    }
    gw->repaint();
}


void GUIcontroller::runCalendar() {
    string APP_Name = "Calandar";

    Process CalandarProc = mo->create_process(APP_Name,"Basic",1,524288);
    if (! mo->In_Memory(CalandarProc)) {
    mo->Add_to_memory(CalandarProc);
    MemoryProcess();
    }

    GImage * cal = new GImage("calendar_back.png", 1.3*X/5, Y/50);
    cal->setWidth(0.3472*X);
    cal->setHeight(0.3909*Y);
    gw->add(cal);
    GContainer * con_cal0 = new GContainer(GContainer::LAYOUT_GRID, 1, 4);
    con_cal0->setX(1.53*X / 5);
    con_cal0->setY(Y / 17);
    con_cal0->setHeight(0.05 * Y);
    con_cal0->setWidth(X / 4);
    GButton * previous = new GButton("prev");
    previous -> setActionCommand("prev");
    GButton * next = new GButton("next");
    next->setActionCommand("next");
    GChooser * yearc = new GChooser();
    GChooser * monthc = new GChooser();

    yearc->addItems({"2021","2020","2019","2018","2017","2016","2015"});

    monthc->addItems({"1","2","3","4","5","6","7","8","9","10","11","12"});
    con_cal0->add(previous);
    con_cal0->add(yearc);
    con_cal0->add(monthc);
    con_cal0->add(next);


    GContainer * con_cal1 = new GContainer();
    con_cal1 -> setX(1.9*X/5);
    con_cal1 -> setY(Y/10);
    con_cal1 -> setWidth(X/4);
    con_cal1 -> setHeight(Y*0.3);
    //GTextArea * window = new GTextArea();
    GLabel * window = new GLabel();
    //window -> setRows(12);
    window -> setWidth(1.5*X);
    //window -> setEditable(false);
    con_cal1 -> addToRegion(window,GContainer::REGION_CENTER);

    GContainer * con_cal2 = new GContainer();
    con_cal2->setX(1.53*X / 5);
    con_cal2->setY(4.3*Y / 13);
    con_cal2->setHeight(0.05 * Y);
    con_cal2->setWidth(X / 4);
    GButton * exit = new GButton("exit");
    con_cal2 -> add(exit);

    GButton * min = new GButton("min");
    con_cal2 -> add(min);

    sCalendar * calendar = new sCalendar();
    yearc->setSelectedItem("2020");
    monthc->setSelectedItem(to_string(calendar->get_this_month()));

    string command;
    yearc->setEditable(true);
    monthc->setEditable(true);
    previous->setEnabled(true);
    next->setEnabled(true);

    while (true) {
        window->setText(calendar->showMonth(atoi(yearc->getSelectedItem().c_str()), atoi(monthc->getSelectedItem().c_str())));
        GEvent e = waitForEvent(CLICK_EVENT | ACTION_EVENT);
        command = e.getActionCommand();
        if (command.length() > 0){
            if (!command.compare("prev")) {
                int mon = atoi(monthc->getSelectedItem().c_str()) - 1;
                int yea = atoi(yearc->getSelectedItem().c_str());
                yea = mon == 0 ? yea - 1 : yea;
                mon = mon == 0 ? 12 : mon;
                monthc->setSelectedItem(to_string(mon));
                yearc->setSelectedItem(to_string(yea));

                string process_name = "prev";
                cala_process_index += 1;
                MemoryAdd(APP_Name,process_name,cala_process_index,262144);


            } else if (!command.compare("next")) {
                int mon = atoi(monthc->getSelectedItem().c_str()) + 1;
                int yea = atoi(yearc->getSelectedItem().c_str());
                yea = mon == 13 ? yea + 1 : yea;
                mon = mon == 13 ? 1 : mon;
                monthc -> setSelectedItem(to_string(mon));
                yearc -> setSelectedItem(to_string(yea));

                string process_name = "next";
                cala_process_index += 1;
                MemoryAdd(APP_Name,process_name,cala_process_index,262144);

            } else if (!command.compare("exit")) {
                con_cal0->setVisible(false);
                con_cal1->setVisible(false);
                cal->setVisible(false);
                con_cal2->setVisible(false);

                MemoryQuit("Calandar");

                return;
            }
            else if (!command.compare("min")) {
                            con_cal0->setVisible(false);
                            con_cal1->setVisible(false);
                            cal->setVisible(false);
                            con_cal2->setVisible(false);
                            return;
            }
        }
    }

}


void GUIcontroller::runCalculator() {
    string APP_Name = "Calculator";
    Process CalculatorProc = mo->create_process(APP_Name,"Basic",1,524288);
    if (! mo->In_Memory(CalculatorProc)) {
    mo->Add_to_memory(CalculatorProc);
    MemoryProcess();
    }

    GImage * calculator = new GImage("calculator.png",1.3*X/5,Y/60);
    calculator->setWidth(0.34722*X);
    calculator->setHeight(0.7669*Y);
    gw->add(calculator);
    GContainer * con_cal0 = new GContainer();
    con_cal0->setX(1.53*X/5);
    con_cal0->setY(Y/17);
    con_cal0->setHeight(0.05*Y);
    con_cal0->setWidth(X/4);
    GContainer * con_cal = new GContainer(GContainer::LAYOUT_GRID,5,4);
    con_cal->setX(1.53*X/5);
    con_cal->setY(Y/6);
    con_cal->setHeight(0.6*Y);
    con_cal->setWidth(X/4);

    GLabel * window = new GLabel();
    QFont font;
    font.setBold(true);
    font.setPointSize(20);
    window->setFont(font);
    window->setBounds(1.3*X/5,Y/17,X/4,Y/20);
    con_cal0->add(window);

    GButton * bac = new GButton("ac","ac.png");
    bac->setActionCommand("reset");
    GButton * blb = new GButton("","left.png");
    blb->setActionCommand("(");
    GButton * brb = new GButton("","right.png");
    brb->setActionCommand(")");
    GButton * bre = new GButton("","re.png");
    bre->setActionCommand("regret");
    GButton * b7 = new GButton("","7.png");
    b7->setActionCommand("7");
    GButton * b8 = new GButton("","8.png");
    b8->setActionCommand("8");
    GButton * b9 = new GButton("","9.png");
    b9->setActionCommand("9");
    GButton * bdi = new GButton("","divide.png");
    bdi->setActionCommand("/");
    GButton * b4 = new GButton("","4.png");
    b4->setActionCommand("4");
    GButton * b5 = new GButton("","5.png");
    b5->setActionCommand("5");
    GButton * b6 = new GButton("","6.png");
    b6->setActionCommand("6");
    GButton * bml = new GButton("","multi.png");
    bml->setActionCommand("*");
    GButton * b1 = new GButton("","1.png");
    b1->setActionCommand("1");
    GButton * b2 = new GButton("","2.png");
    b2->setActionCommand("2");
    GButton * b3 = new GButton("","3.png");
    b3->setActionCommand("3");
    GButton * bmi = new GButton("","minus.png");
    bmi->setActionCommand("-");
    GButton * b0= new GButton("","0.png");
    b0->setActionCommand("0");
    GButton * bdo = new GButton("","dot.png");
    bdo->setActionCommand(".");
    GButton * beq = new GButton("","equal.png");
    beq->setActionCommand("solve");
    GButton * bad = new GButton("","sum.png");
    bad->setActionCommand("+");
    con_cal->add(bac);
    con_cal->add(blb);
    con_cal->add(brb);
    con_cal->add(bre);
    con_cal->add(b7);
    con_cal->add(b8);
    con_cal->add(b9);
    con_cal->add(bdi);
    con_cal->add(b4);
    con_cal->add(b5);
    con_cal->add(b6);
    con_cal->add(bml);
    con_cal->add(b1);
    con_cal->add(b2);
    con_cal->add(b3);
    con_cal->add(bmi);
    con_cal->add(b0);
    con_cal->add(bdo);
    con_cal->add(beq);
    con_cal->add(bad);
    GButton * exit = new GButton("exit");
    con_cal->addToRegion(exit,GContainer::REGION_SOUTH);
    GButton * min = new GButton("min");
    con_cal->addToRegion(min,GContainer::REGION_SOUTH);

    Calculator * cal = new Calculator();
    Set<string> nums = {"0","1","2","3","4","5","6","7","8","9","+","-","*","/",".","(",")"};
    string operation;
    while(true) {
        string command = "";
        GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
        command = e.getActionCommand();
        if (command.length()>0) {
            if (!command.compare("reset")){
                window->setText("");
                operation = "";
            } else if (!command.compare("regret")){
                operation = operation.substr(0,operation.length()-1);
                window->setText(operation);
            } else if (!command.compare("solve")){
                string result = "=";
                result = cal->run(operation);
                window->setText(result);
                operation = "";
                cal = new Calculator();

                string process_name = "Solve";
                calc_process_index += 1;
                MemoryAdd(APP_Name,process_name,calc_process_index,262144);

            } else if (nums.contains(command)){
                operation.append(command);
                window->setText(operation);
            } else if (!command.compare("exit")) {
                calculator->setVisible(false);
                con_cal->setVisible(false);
                con_cal0->setVisible(false);
                window->setVisible(false);

                MemoryQuit("Calculator");

                return;               
            }
            else if (!command.compare("min")) {
                            calculator->setVisible(false);
                            con_cal->setVisible(false);
                            con_cal0->setVisible(false);
                            window->setVisible(false);
                            return;
        }
    }

}
}

void GUIcontroller::dealFileSystem() {

    int process_index = 0;
    string APP_Name = "File System";

    string operant = input_oper->getSelectedItem();

    if (!operant.compare("touch")){
        try {
            fs->ch_user(current_user);
            if (thread->has_wr(current_user)) {

                string filename = input_file->getText();
                GContainer * temp = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL,2,3);
                temp->setX(X/4);
                temp->setY(Y/10);
                temp->setWidth(X/3);
                temp->setHeight(Y/2);
                temp->setBackground(100);
                GTextArea * content_area = new GTextArea();
                content_area->setEditable(true);
                content_area->setWidth(1.63*X/5);
                content_area->setRows(20);
                GButton * enter = new GButton("Enter");
                enter->setActionCommand("enter");
                GButton * exit = new GButton("Exit");
                exit->setActionCommand("exit");
                GButton * revoke = new GButton("Revoke");
                revoke->setActionCommand("revoke");
                temp->addToGrid(content_area,0,0);
                temp->addToGrid(enter,1,0);
                temp->addToGrid(revoke,1,1);
                temp->addToGrid(exit,1,2);

                string Process_Name = "Touch";
                process_index += 1;
                Process Touch = mo->create_process(APP_Name,Process_Name,process_index,524288);
                mo->Add_to_memory(Touch);
                MemoryProcess();


                string complete;
                int revoke_time = 0;
                int enter_time = 0;
                string content;

                while(true) {
                    GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
                    complete = e.getActionCommand();

                    if (complete.compare("enter")==0 & enter_time == 0) {
                        revoke_time = 0;
                        content = content_area->getText();
                        fs->touch(filename,content,7);
                        enter_time += 1;
                        ls->setText(fs->ls());
                        file_info->setText(fs->file_info(filename));
                    }
                    if (complete.compare("enter")==0 & enter_time > 0){
                        fs->changeContent(filename,content);
                        file_info->setText(fs->file_info(filename));
                    }
                    if (complete.compare("revoke")==0 & content.compare(content_area->getText()) != 0) {
                        content_area->setText(content);
                    }
                    if (complete.compare("revoke")==0 & content.compare(content_area->getText()) == 0 & revoke_time == 0) {
                        revoke_time += 1;
                        fs->revoke(filename);
                        content_area->setText(fs->cat(filename));
                        file_info->setText(fs->file_info(filename));
                    }
                    if (!complete.compare("exit")){
                        file_info->clearText();
                        temp->clear();
                        temp->setVisible(false);
                        temp = nullptr;

                        mo->Remove_from_memory(Touch);
                        MemoryProcess();

                        break;
                    }
                }

            } else {
                set_reporter(reporter_wrlock, "You don't have right to write.", wrinfo);
            }

        } catch (ErrorException e) {

            string message = e.getMessage().append("\n");
            set_reporter(reporter_error,message,poolinfo);
        }
    } else if (!operant.compare("cd")){
        fs->ch_user(current_user);
        try {

//            string Process_Name = "Cd";
//            process_index += 1;
//            Process cd = mo->create_process(APP_Name,Process_Name,process_index,262144);
//            mo->Add_to_memory(cd);
//            MemoryProcess();

            string dir_name = input_file->getText();
            fs->cd(dir_name);
            pwd->setText(fs->pwd());
            ls->setText(fs->ls());
        } catch (ErrorException &e) {
            string message = e.getMessage().append("\n");
            set_reporter(reporter_error,message,poolinfo);
        }


    } else if (!operant.compare("mkdir")){
        fs->ch_user(current_user);
        string dir_name = input_file->getText();
        try {
            if (thread->has_wr(current_user)) {

//                string Process_Name = "Mkdir";
//                process_index += 1;
//                Process mkdir = mo->create_process(APP_Name,Process_Name,process_index,262144);
//                mo->Add_to_memory(mkdir);
//                MemoryProcess();

                fs->mkdir(dir_name);
                ls->setText(fs->ls());
            }
        } catch (ErrorException e) {
            string message = e.getMessage().append("\n");
            set_reporter(reporter_error,message,poolinfo);

        }
    } else if (!operant.compare("open")){
        fs->ch_user(current_user);

        string filename = input_file->getText();
        GContainer * temp = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL,2,3);
        temp->setX(X/4);
        temp->setY(Y/10);
        temp->setWidth(X/3);
        temp->setHeight(Y/2);
        temp->setBackground(100);
        GTextArea * content_area = new GTextArea();
        content_area->setEditable(true);
        content_area->setWidth(1.63*X/5);
        content_area->setRows(20);
        GButton * enter = new GButton("Enter");
        enter->setActionCommand("enter");
        GButton * exit = new GButton("Exit");
        exit->setActionCommand("exit");
        GButton * revoke = new GButton("Revoke");
        revoke->setActionCommand("revoke");
        temp->addToGrid(content_area,0,0);
        temp->addToGrid(enter,1,0);
        temp->addToGrid(revoke,1,1);
        temp->addToGrid(exit,1,2);


        if (thread->has_wr(current_user)) {

            string Process_Name = "Open";
            process_index += 1;
            Process open = mo->create_process(APP_Name,Process_Name,process_index,524288);
            mo->Add_to_memory(open);
            MemoryProcess();

            string complete;
            int revoke_time = 0;
            int enter_time = 0;
            string content = fs->cat(filename);
            file_info->setText(fs->file_info(filename));

            content_area->setText(content);
            while(true) {
                try{
                    GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
                    complete = e.getActionCommand();

                    if (complete.compare("enter")==0){
                        enter_time += 1;
                        fs->changeContent(filename,content);
                        file_info->setText(fs->file_info(filename));
                    }
                    if (complete.compare("revoke")==0 & content.compare(content_area->getText()) != 0) {
                        content_area->setText(content);
                    }
                    if (complete.compare("revoke")==0 & content.compare(content_area->getText()) == 0 & revoke_time == 0 && enter_time > 0) {
                        revoke_time += 1;
                        fs->revoke(filename);
                        content_area->setText(fs->cat(filename));
                        file_info->setText(fs->file_info(filename));
                    }
                    if (!complete.compare("exit")){
                        file_info->clearText();
                        temp->setVisible(false);
                        mo->Remove_from_memory(open);
                        MemoryProcess();
                        break;
                    }

                } catch(ErrorException e) {
                    string message = e.getMessage().append("\n");
                    set_reporter(reporter_error,message,poolinfo);
                }
            }

        } else if (thread->has_rd(current_user)){
            string Process_Name = "Open";
            process_index += 1;
            Process open = mo->create_process(APP_Name,Process_Name,process_index,524288);
            mo->Add_to_memory(open);
            MemoryProcess();

            string complete;
            string content = fs->cat(filename);

            content_area->setText(content);
            content_area->setEditable(false);
            while(true) {
                try {
                    GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
                    complete = e.getActionCommand();
                    if (!complete.compare("exit")){
                        file_info->clearText();
                        temp->setVisible(false);

                        mo->Remove_from_memory(open);
                        MemoryProcess();

                        break;
                    }
                } catch(ErrorException e) {

                    string message = e.getMessage().append("\n");
                    set_reporter(reporter_error,message,poolinfo);

                }
            }

        }


    } else if (!operant.compare("remove")) {
        fs->ch_user(current_user);
        try {
            string filename = input_file->getText();
            if (filename.find(".") != filename.npos){
                fs->rm(filename);
                ls->setText(fs->ls());

//                string Process_Name = "Rm";
//                process_index += 1;
//                Process rm = mo->create_process(APP_Name,Process_Name,process_index,262144);
//                mo->Add_to_memory(rm);
//                MemoryProcess();

            } else {
                fs->rm(filename, "-r");
                ls->setText(fs->ls());

//                string Process_Name = "Rm";
//                process_index += 1;
//                Process rm = mo->create_process(APP_Name,Process_Name,process_index,262144);
//                mo->Add_to_memory(rm);
//                MemoryProcess();
            }

        } catch (ErrorException e) {

            string message = e.getMessage().append("\n");
            set_reporter(reporter_error,message,poolinfo);

        }
    } else if (!operant.compare("copy")) {
        fs->ch_user(current_user);
        string filename = input_file->getText();
        while (true) {
            try {
                if (filename.length() == 0) {
                    return;
                }
                if (filename.find(".") != filename.npos) {
                    file_info->setText(fs->file_info(filename));
                    break;
                } else {
                    if (fs->has_path(filename)) break;
                    set_reporter(reporter_error, "Could not find directory.\n",poolinfo);
                }
            } catch (ErrorException e) {
                set_reporter(reporter_error, e.getMessage().append("\n"),poolinfo);
            }
        }

        input_file->setText("Please input your target directory here. ");
        string complete;

        string Process_Name = "Copy";
        process_index += 1;
        Process copy = mo->create_process(APP_Name,Process_Name,process_index,524288);
        mo->Add_to_memory(copy);
        MemoryProcess();

        while (true) {
            try {
                oper->setText("copy!");
                GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
                complete = e.getActionCommand();
                if (!complete.compare("copy!")) {
                    string target = input_file->getText();
                    if (target.length() == 0) {
                        oper->setText("run");
                        file_info->clearText();
                        break;
                    } else if (filename.rfind(".") != filename.npos) {
                        fs->cp(filename, target, "-p");
                        fs->cd(target);
                        pwd->setText(fs->pwd());
                        ls->setText(fs->ls());
                        oper->setText("run");

                        mo->Remove_from_memory(copy);
                        MemoryProcess();

                        break;
                    } else {
                        fs->cp(filename, target, "-r");
                        fs->cd(target);
                        pwd->setText(fs->pwd());
                        ls->setText(fs->ls());
                        oper->setText("run");

                        mo->Remove_from_memory(copy);
                        MemoryProcess();

                        break;
                    }

                }
            } catch (ErrorException e) {
                string message = e.getMessage().append("\n");
                set_reporter(reporter_error,message,poolinfo);
            }
        }

    } else if (!operant.compare("move")) {
        fs->ch_user(current_user);
        string filename = input_file->getText();
        while (true) {
            try {
                if (filename.length() == 0) {
                    return;
                }
                if (filename.find(".") != filename.npos) {
                    file_info->setText(fs->file_info(filename));
                    break;
                } else {
                    if (fs->has_path(filename)) break;
                    set_reporter(reporter_error, "Could not find directory.\n",poolinfo);
                }
            } catch (ErrorException e) {
                set_reporter(reporter_error, e.getMessage().append("\n"),poolinfo);
            }
        }
        input_file->setText("Please input your target directory here. ");
        string complete;

        string Process_Name = "Move";
        process_index += 1;
        Process move = mo->create_process(APP_Name,Process_Name,process_index,524288);
        mo->Add_to_memory(move);
        MemoryProcess();


        while (true) {
            try {
                oper->setText("move!");
                GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
                complete = e.getActionCommand();
                if (!complete.compare("move!")){
                    string target = input_file->getText();
                    if (target.length() == 0) {
                        oper->setText("run");
                        file_info->clearText();
                        break;
                    } else if (filename.rfind(".") != filename.npos){
                        fs->mv(filename,target,"-p");
                        fs->cd(target);
                        pwd->setText(fs->pwd());
                        ls->setText(fs->ls());
                        oper->setText("run");

                        mo->Remove_from_memory(move);
                        MemoryProcess();

                        break;
                    } else {
                        fs->mv(filename,target,"-r");
                        fs->cd(target);
                        pwd->setText(fs->pwd());
                        ls->setText(fs->ls());
                        oper->setText("run");

                        mo->Remove_from_memory(move);
                        MemoryProcess();
                        break;
                    }
                }
            } catch (ErrorException e) {
                string message = e.getMessage().append("\n");
                set_reporter(reporter_error,message,poolinfo);
            }
        }

    } else if (!operant.compare("find")) {
        fs->ch_user(current_user);
        try {
            string filename = input_file->getText();
            ls->setText(fs->find(filename));
        } catch (ErrorException e) {
            set_reporter(reporter_error,e.getMessage().append("\n"),poolinfo);
        }
    } else if (!operant.compare("chmod")) {
        fs->ch_user(current_user);

        string filename = input_file->getText();
        input_file->setText("Mod here.");
        string complete;

        string Process_Name = "Chmod";
        process_index += 1;
        Process chmod = mo->create_process(APP_Name,Process_Name,process_index,524288);
        mo->Add_to_memory(chmod);
        MemoryProcess();

        while (true) {
            try {
                oper->setText("chmod!");
                GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
                complete = e.getActionCommand();
                if (!complete.compare("chmod!")){
                    assert(atoi(input_file->getText().c_str()));
                    int target = atoi(input_file->getText().c_str());
                    fs->chmod(filename,target);

                    mo->Remove_from_memory(chmod);
                    MemoryProcess();

                    file_info->setText(fs->file_info(filename));
                    oper->setText("run");

                    break;
                }
            } catch (ErrorException e) {
                string message = e.getMessage().append("\n");
                set_reporter(reporter_error,message,poolinfo);
            }
        }
    }
}

string GUIcontroller::set_reporter(GTextArea * reporter, string toadd, string info){
    if (rows > 6) {
        string information = toadd;
        rows = 1;
        reporter->setText(information);
        return information;
    } else {
        rows += 1;
        info.append(toadd);
        reporter->setText(info);
        return info;
    }
}

void GUIcontroller::_widget(){
    X = gw->getWidth();
    Y = gw->getHeight();
    gw->setResizable(true);
    GImage * back_ground = new GImage("background.png");

    back_ground->setWidth(X);
    back_ground->setHeight(Y);

    back_ground->sendToBack();
    gw->add(back_ground);
    gw->setTitle("Simulated Operating System");


}

void GUIcontroller::initThread() {
    Process OS_1 = mo->create_process("OS","Thread",1,524288);
    mo->Add_to_memory(OS_1);


    command.add("wrlock", 1);
    command.add("rdlock", 2);
    command.add("unlock", 3);


    //w r lock
    GContainer * thread_lay1 = new GContainer;
    thread_lay1->setBounds(1.3*X/200,Y*0.919,0.98*X/5,1*Y/18);
    pick_user = new GChooser();

    pick_user->setBounds(X/144,Y,1.3*X/20,Y/40);

    pick_user->addItem(current_user);

    write = new GButton("write");
    write->setActionCommand("wrlock");
    read = new GButton("read");
    read ->setActionCommand("rdlock");
    unlock = new GButton("unlock");
    unlock->setActionCommand("unlock");


    thread_lay1->add(pick_user);
    thread_lay1->add(write);
    thread_lay1->add(read);
    thread_lay1->add(unlock);


    //r-w-reporter
    GContainer * thread_lay2 = new GContainer();
    reporter_wrlock = new GTextArea();
    thread_lay2->setBounds(0.208*X,Y/5*3.99,X*0.287,1.07*Y/6);
    reporter_wrlock->setRows(10);
    reporter_wrlock->setEditable(false);
    reporter_wrlock->setWidth(X*0.284);
    wrinfo = "Initialize the reader-writer lock reporter.\n";
    reporter_wrlock->setText(wrinfo);
    rows = 1;
    thread_lay2->add(reporter_wrlock);



    //error reporter
    GContainer * thread_lay3 = new GContainer();
    thread_lay3->setBounds(X*0.502,Y/5*3.99,X*0.287,1.07*Y/6);
    reporter_error = new GTextArea();
    reporter_error->setRows(10);
    reporter_error->setEditable(false);
    reporter_error->setWidth(X*0.284);
    poolinfo = "Initialize the error reporter.\n";
    reporter_error->setText(poolinfo);
    thread_lay3->add(reporter_error);





    thread = new spthread(current_user, false, false);

    wrinfo.append("Initialize a thread for ");
    wrinfo.append(current_user);
    wrinfo.append("\n");
    reporter_wrlock->setText(wrinfo);

}


void GUIcontroller::initFileSystem() {
    Process initFS = mo->create_process("File System","Init",1,1048576);
    if (! mo->In_Memory(initFS)) {
        mo->Add_to_memory(initFS);
        MemoryProcess();
    }


    command.add("run", 4);

    GContainer * con_pwd = new GContainer();
    pwd = new GLabel("/root");
    QFont font;
    font.setBold(true);
    font.setPointSize(20);
    pwd->setFont(font);
    con_pwd->setBounds(X/200,0.5*Y/30,X/5,1.89/36*Y);
    con_pwd->add(pwd);

    GContainer * con_ls = new GContainer();
    ls = new GTextArea();
    ls->setRows(15);
    ls->setWidth(X/5);
    con_ls->setX(0);
    con_ls->setY(1.9*Y/30);
    con_ls->setWidth(X/5);
    con_ls->setHeight(1.4*Y/4);
    con_ls->add(ls);

    GContainer * con_info = new GContainer();
    file_info = new GTextArea();
    file_info->setRows(13);
    file_info->setWidth(X/5);
    //con_info->setBackground();
    con_info->setX(0);
    con_info->setY(0.375*Y);
    con_info->setWidth(X/5);
    con_info->setHeight(1.4*Y/4);
    con_info->add(file_info);


    GContainer * con_oper = new GContainer();
    input_file = new GTextField();
    input_file->setEditable(true);
    input_file->setWidth(10/X);
    input_file->setBackground("Blue");

    input_oper = new GChooser;
    input_oper->addItem("touch");
    input_oper->addItem("mkdir");
    input_oper->addItem("open");
    input_oper->addItem("remove");
    input_oper->addItem("copy");
    input_oper->addItem("move");
    input_oper->addItem("cd");
    input_oper->addItem("find");
    input_oper->addItem("chmod");

    oper = new GButton("run");

    input_file = new GTextField;
    input_file->setWidth(X/10);

    con_oper->setX(0.003*X);
    con_oper->setY(0.7*Y);
    con_oper->setWidth(X/5);
    con_oper->setHeight(1.4*Y/20);

    con_oper->add(input_file);
    con_oper->add(input_oper);
    con_oper->add(oper);



    fs = new sFileSystem(current_user);



}

void GUIcontroller::initMain(){
    Process OS_1 = mo->create_process("OS","Main",1,524288);
    mo->Add_to_memory(OS_1);
    command.add("openfs",5);
    command.add("openmemory",6);
    command.add("opencalculator",7);
    command.add("opencalendar",8);
    command.add("exit",99);

    table = new GImage("main.png");
    table->setWidth(3*X/5);
    table->setHeight(23*Y/30);
    gw->add(table,X/5,0);
    GContainer * con_main = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);

    con_main->setX(3.5*X/5);

    con_main->setY(Y/30);
    con_main->setWidth(X/10);
    con_main->setHeight(Y/2);
    con_main->setSpacing(Y/60);

    fileb = new GButton("","file_icon.png");

    fileb->setActionCommand("openfs");
    memoryb = new GButton("","memory_icon.png");
    memoryb->setActionCommand("openmemory");
    calculatorb = new GButton("","calculator_icon.png");
    calculatorb->setActionCommand("opencalculator");
    calendarb = new GButton("","calendar_icon.png");
    calendarb->setActionCommand("opencalendar");
    exitb = new GButton("","exit.png");
    exitb->setActionCommand("exit");
    con_main->add(calendarb);
    con_main->add(calculatorb);
    con_main->add(fileb);
    con_main->add(memoryb);
    con_main->add(exitb);
}

void GUIcontroller::init_login() {
    Process OS_1 = mo->create_process("OS","Login",1,524288);
    mo->Add_to_memory(OS_1);

    command.add("confirm_user", 0);
    login.add("Jacy","abc");
    login.add("Eric","abc");
    login.add("Blaine","abc");
    login.add("Yuheng","abc");

    //log in
    GContainer * thread_lay0 = new GContainer;
    thread_lay0->setX(1.3*X/200);
    thread_lay0->setY(Y/5*4.03);
    thread_lay0->setWidth(0.98*X/5);
    thread_lay0->setHeight(Y/18);
    GLabel *name = new GLabel("name:");
    input_name = new GTextField();
    GLabel *password = new GLabel("password:");
    input_word = new GTextField();


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
    confirm_user->setActionCommand("confirm_user");
    thread_lay01->add(confirm_user);

    thread_lay0->add(name);
    thread_lay0->add(input_name);
    thread_lay0->add(password);
    thread_lay0->add(input_word);


}

void GUIcontroller::initMemory() {
    static const int GBtoKB = 1048576;

    // Create the process called OS.
    Process OS_1 = mo->create_process("OS","Memory",1,524288);
    mo->Add_to_memory(OS_1);

    int Linespace = Y/25;
    int Colspace = X/10;

    // Top right Block of memory GUI
    GContainer * memory_lay_top0 = new GContainer;
    memory_lay_top0->setX(4*X/5);
    memory_lay_top0->setY(Y/200);
    memory_lay_top0->setWidth(X/5);
    memory_lay_top0->setHeight(Y/18);
    GLabel *memory_title1 = new GLabel("Memory Summary Table");
    memory_title1->setColor("Black");
    memory_lay_top0->add(memory_title1);

    GContainer * memory_lay_top1 = new GContainer;
    memory_lay_top1->setX(4*X/5);
    memory_lay_top1->setY(Y/200 + Linespace);
    memory_lay_top1->setWidth(Colspace);
    memory_lay_top1->setHeight(Linespace);
    GLabel *memory_title2 = new GLabel("Physical memory:");
    memory_title2->setColor("Black");
    memory_lay_top1->add(memory_title2);

    GContainer * memory_lay_top2 = new GContainer;
    memory_lay_top2->setX(4*X/5 + Colspace);
    memory_lay_top2->setY(Y/200 + Linespace);
    memory_lay_top2->setWidth(Colspace);
    memory_lay_top2->setHeight(Linespace);
    GLabel *memory_content1 = new GLabel("4.00GB");
    memory_content1->setColor("Black");
    memory_lay_top2->add(memory_content1);

    GContainer * memory_lay_top3 = new GContainer;
    memory_lay_top3->setX(4*X/5);
    memory_lay_top3->setY(Y/200 + Linespace*2);
    memory_lay_top3->setWidth(Colspace);
    memory_lay_top3->setHeight(Linespace);
    GLabel *memory_title3 = new GLabel("Memory used:");
    memory_title3->setColor("Black");
    memory_lay_top3->add(memory_title3);

    memory_lay_top4 = new GContainer;
    memory_lay_top4->setX(4*X/5 + Colspace);
    memory_lay_top4->setY(Y/200 + Linespace*2);
    memory_lay_top4->setWidth(Colspace/2);
    memory_lay_top4->setHeight(Linespace);

    // Get info from mo
    int occupied_size = mo->get_occupied_memory_size();
    int mid = occupied_size*100/GBtoKB;
    // Keep 2 decimal place.
    int mid_part1 = mid/100; // eg.For 3.92 this variable == 3.
    int mid_part2 = mid%100; // this variable == 92.
    string cbn = integerToString(mid_part1)+"."+integerToString(mid_part2);
    memory_content2 = new GLabel(cbn+"GB");
    if (mid_part1 < 3){
    memory_content2->setColor("Black");
    }
    else{
    memory_content2->setColor("Red");
    }
    memory_lay_top4->add(memory_content2);

    GContainer * memory_lay_top4_1 = new GContainer;
    memory_lay_top4_1->setX(4*X/5 + Colspace*1.45);
    memory_lay_top4_1->setY(Y/200 + Linespace*2);
    memory_lay_top4_1->setWidth(Colspace/2);
    memory_lay_top4_1->setHeight(Linespace);
    GLabel *memory_content2_1 = new GLabel("/4.00GB");
    memory_content2_1->setColor("Black");
    memory_lay_top4_1->add(memory_content2_1);

    GContainer * memory_lay_top5 = new GContainer;
    memory_lay_top5->setX(4*X/5);
    memory_lay_top5->setY(Y/200 + Linespace*3);
    memory_lay_top5->setWidth(Colspace*1.1);
    memory_lay_top5->setHeight(Linespace);
    GLabel *memory_title4 = new GLabel("Virtual memory invoked:");
    memory_title4->setColor("Black");
    memory_lay_top5->add(memory_title4);


    memory_lay_top6 = new GContainer;
    memory_lay_top6->setX(4*X/5 + Colspace);
    memory_lay_top6->setY(Y/200 + Linespace*3);
    memory_lay_top6->setWidth(Colspace);
    memory_lay_top6->setHeight(Linespace);
    string invoked;
    if (occupied_size == 4194304) invoked = "Yes";
    else invoked = "No";
    memory_content3 = new GLabel(invoked);
    memory_content3->setColor("Black");
    memory_lay_top6->add(memory_content3);



    // Top Middle Block of memory GUI
    GContainer * memory_lay_mid0 = new GContainer;
    memory_lay_mid0->setX(4*X/5);
    memory_lay_mid0->setY(Y/200*40);
    memory_lay_mid0->setWidth(X/5 - Colspace*1.3);
    memory_lay_mid0->setHeight(Y/18);
    GLabel *memory_title5 = new GLabel("Memory Table");
    memory_title5->setColor("Black");
    memory_lay_mid0->add(memory_title5);

    // Memory Button
    memory_button = new GContainer;

    memory_button->setX(X-Colspace*1.3);
    memory_button->setY(Y/200*40);
    memory_button->setWidth(Colspace*0.9);
    memory_button->setHeight(Y/18);

    SelectButton = new GChooser;
    SelectButton->addItem("Process");
    SelectButton->addItem("APP");


    memory_button->add(SelectButton);

    GContainer *OKArea = new GContainer;
    OKArea->setX(X-Colspace*0.4);
    OKArea->setY(Y/200*40);
    OKArea->setWidth(Colspace*0.4);
    OKArea->setHeight(Y/18);

    OK = new GButton("OK");
    OK->setActionCommand("OK");
    command["OK"] = 9;
    OKArea->add(OK);






    // Memory Table
    memory_table = new GContainer;

    Vector<Process> Current_Process = mo->get_current_process();
    int num_process = Current_Process.size();
    mt= new GTable(num_process,5);
    mt->setColumnHeaderStyle(GTable::COLUMN_HEADER_NONE);
    for (int i = 0; i < num_process; i++){
       mt->set(i,0,Current_Process[i].App_name);
       mt->set(i,1,Current_Process[i].process_name);
       mt->set(i,2,integerToString(Current_Process[i].process));
       mt->set(i,3,integerToString(Current_Process[i].memory_declared));
       mt->set(i,4,integerToString(mo->Get_process_memory(Current_Process[i])));
    }


    // 这里的表格会出现间距太窄的问题，这里还没有修复，请往下在这个区域内增加内容


    //


    memory_table->setX(4*X/5);
    memory_table->setY(Y*23/90);
    memory_table->setWidth(X/5);
    memory_table->setHeight(Linespace*12.5);
    memory_table->add(mt);

    // 表格的Header还未加上去！！！！！！
    // APP Name ; Process name ; memory decalred ; memory occupied (HEADER)
//    GContainer * header = new GContainer;
//    header->setX(4*X/5);
//    header->setY(Y*23/90);
//    header->setWidth(X/5);
//    header->setHeight(Linespace*12.5);
//    header->setColor("Black");


    // The bottom part of the memory
    int start_x = 4*X/5*1.008;
    int start_y = Y/200*165;
    int length = X/5*0.9;
    int height = Y/18+Linespace*1.5;


    GRect * memory_bar = new GRect(start_x,start_y,length,height);
    memory_bar->sendToBack();
    memory_bar->setFilled(1);
    memory_bar->setFillColor(255,255,255);
    memory_bar->setColor(255,255,255);
    gw->add(memory_bar);


    // Draw memory bar for every process.
    for (Process process:mo->get_current_process()){
        // Draw memory bar for every block.
        for (Block block:mo->Get_address(process)){
            Vector<double> proportion = mo->block_position_transfer(block);
            double start_proportion = proportion[0];
            double terminate_proportion = proportion[1];
            double size_proportion = terminate_proportion - start_proportion;
            APP_bar = new GRect(start_x + start_proportion*length,start_y,length*size_proportion,height);
            APP_bar->sendToFront();
            APP_bar->setFilled(1);
            APP_bar->setColor(255,255,255);
            APP_Bars.add(APP_bar);
            if (process.App_name == "File System") APP_bar->setFillColor(255,165,0);
            else if (process.App_name == "Calculator") APP_bar->setFillColor(0,156,255);
            else if (process.App_name == "Calandar") APP_bar->setFillColor(36,110,0);
            else if (process.App_name == "OS") APP_bar->setFillColor(166,0,200);
            gw->add(APP_bar);
        }
    }



    // Legend. (This part is already well-set, please do not modify!)

    // File System
    GRect * APP_legend1 = new GRect(start_x,start_y+height+0.5*Linespace,length/20,length/20);
    APP_legend1->sendToFront();
    APP_legend1->setFilled(1);
    APP_legend1->setFillColor(255,165,0);
    APP_legend1->setColor(255,165,0);
    gw->add(APP_legend1);
    GContainer * LL1 = new GContainer;
    LL1->setBounds(start_x+length/20,start_y+height*0.9,length/5,length/5);
    GLabel * LabelLegend1 = new GLabel("File\nSystem");
    LabelLegend1->setSize(length/5,length/5);
    LL1->add(LabelLegend1);

    // Calculator
    int horizontal = length/20+length/4;
    GRect * APP_legend2 = new GRect(start_x+horizontal,start_y+height+0.5*Linespace,length/20,length/20);
    APP_legend2->sendToFront();
    APP_legend2->setFilled(1);
    APP_legend2->setFillColor(0,156,255);
    APP_legend2->setColor(0,156,255);
    gw->add(APP_legend2);
    GContainer * LL2 = new GContainer;
    LL2->setBounds(start_x+horizontal+length/20,start_y+height*0.9,length/5,length/5);
    GLabel * LabelLegend2 = new GLabel("Calc");
    LabelLegend2->setSize(length/5,length/5);
    LL2->add(LabelLegend2);

    // Calandar
    GRect * APP_legend3 = new GRect(start_x+horizontal*1.7,start_y+height+0.5*Linespace,length/20,length/20);
    APP_legend3->sendToFront();
    APP_legend3->setFilled(1);
    APP_legend3->setFillColor(36,110,0);
    APP_legend3->setColor(36,110,0);
    gw->add(APP_legend3);
    GContainer * LL3 = new GContainer;
    LL3->setBounds(start_x+horizontal*1.7+length/15,start_y+height*0.9,length/4,length/5);
    GLabel * LabelLegend3 = new GLabel("Calandar");
    LabelLegend3->setSize(length/4,length/5);
    LL3->add(LabelLegend3);

    // OS
    GRect * APP_legend4 = new GRect(start_x+horizontal*2.8,start_y+height+0.5*Linespace,length/20,length/20);
    APP_legend4->sendToFront();
    APP_legend4->setFilled(1);
    APP_legend4->setFillColor(166,0,200);
    APP_legend4->setColor(166,0,200);
    gw->add(APP_legend4);
    GContainer * LL4 = new GContainer;
    LL4->setBounds(start_x+horizontal*2.8+length/15,start_y+height*0.9,length/4,length/5);
    GLabel * LabelLegend4 = new GLabel("OS");
    LabelLegend4->setSize(length/4,length/5);
    LL4->add(LabelLegend4);



}


void GUIcontroller::update() {
    while (true){
        run(gw);
    }
}

bool GUIcontroller::isStop() {
    return !runnable;
}


void GUIcontroller::MemoryProcess(){
    // Renew total memory occupied size in the GUI.

    static const int GBtoKB = 1048576;
    int occupied_size = mo->get_occupied_memory_size();
    int mid = occupied_size*100/GBtoKB;
    // Keep 2 decimal place.
    int mid_part1 = mid/100; // eg.For 3.92 this variable == 3.
    int mid_part2 = mid%100; // this variable == 92.
    string cbn;
    if (mid_part2<10) cbn = integerToString(mid_part1)+".0"+integerToString(mid_part2);
    else cbn = integerToString(mid_part1)+"."+integerToString(mid_part2);
    memory_content2 = new GLabel(cbn+"GB");
    if (mid_part1 < 3){
    memory_content2->setColor("Black");
    }
    else{
    memory_content2->setColor("Red");
    }
    memory_lay_top4->clear();
    memory_lay_top4->add(memory_content2);

    // Renew virtual memory invoked status in the GUI.

    string invoked;
    if (occupied_size == 4194304) invoked = "Yes";
    else invoked = "No";
    memory_content3 = new GLabel(invoked);
    memory_content3->setColor("Black");
    memory_lay_top6->clear();
    memory_lay_top6->add(memory_content3);

    // Renew the memory-process table
    memory_table->clear();

    Vector<Process> Current_Process = mo->get_current_process();



    if (SelectButton->getSelectedItem() == "APP"){
    Map<string,int> App_numPro;
    Vector<string> APP_list;
    for(Process process : Current_Process){
        string APP_name = process.App_name;
        if (!App_numPro.containsKey(APP_name)) {
            App_numPro[APP_name] = 1;
            APP_list.add(APP_name);
        }
        else  App_numPro[APP_name] += 1;
    }

    int num_app = APP_list.size();

    mt2= new GTable(num_app,3);
    for (int i = 0; i < num_app; i++){
       string APP_name = APP_list[i];
       mt2->set(i,0,APP_name);
       mt2->set(i,1,integerToString(App_numPro[APP_name]));
       mt2->set(i,2,integerToString(mo->Get_App_memory(APP_name)));
    }
    memory_table->add(mt2);
    }



    else{
    int num_process = Current_Process.size();
    mt= new GTable(num_process,5);
    for (int i = 0; i < num_process; i++){
       mt->set(i,0,Current_Process[i].App_name);
       mt->set(i,1,Current_Process[i].process_name);
       mt->set(i,2,integerToString(Current_Process[i].process));
       mt->set(i,3,integerToString(Current_Process[i].memory_declared));
       mt->set(i,4,integerToString(mo->Get_process_memory(Current_Process[i])));
    }
    memory_table->add(mt);
    }

    // The memory bar plot.

    // Draw memory bar for every process.
    int Linespace = Y/25;
    int start_x = 4*X/5*1.008;
    int start_y = Y/200*165;
    int length = X/5*0.9;
    int height = Y/18+Linespace*1.5;

    // Remove the original bars
    for (GRect *bars:APP_Bars) gw->remove(bars);

    // Add new bars.
    for (Process process:mo->get_current_process()){
        // Draw memory bar for every block.
        for (Block block:mo->Get_address(process)){
            Vector<double> proportion = mo->block_position_transfer(block);
            double start_proportion = proportion[0];
            double terminate_proportion = proportion[1];
            double size_proportion = terminate_proportion - start_proportion;
            APP_bar = new GRect(start_x + start_proportion*length,start_y,length*size_proportion,height);
            APP_bar->sendToFront();
            APP_bar->setFilled(1);
            APP_bar->setColor(255,255,255);
            APP_Bars.add(APP_bar);
            if (process.App_name == "File System") APP_bar->setFillColor(255,165,0);
            else if (process.App_name == "Calculator") APP_bar->setFillColor(0,156,255);
            else if (process.App_name == "Calandar") APP_bar->setFillColor(36,110,0);
            else if (process.App_name == "OS") APP_bar->setFillColor(166,0,200);
            gw->add(APP_bar);
        }
    }

}

void GUIcontroller::MemoryAdd(string APP_Name, string Process_Name, int Process_Index, int Memory_Declared){
    Process process = mo->create_process(APP_Name,Process_Name,Process_Index,Memory_Declared);
    mo->Add_to_memory(process);
    MemoryProcess();
}

void GUIcontroller::MemoryQuit(string APP_Name){
    mo->remove_app_from_memory(APP_Name);
    MemoryProcess();
}
