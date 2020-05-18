#include "sFileSystem.h"
#include <string>
#include <iostream>
#include <error.h>
#include "spthread.h"
#include <gwindow.h>
#include <gbutton.h>
#include <gtextfield.h>
#include "guicontroller.h"

using namespace std;
/*
void main() {

    sFileSystem system = sFileSystem("Jacy");
    cout << "************************" << endl;
    //system.pwd();
    system.touch("foo.txt","foo",7);
    system.touch("foo2.txt","Hello world",4);
    system.touch("hello.txt","NoNoNo.",7);
    system.mkdir("dev1");
    system.mkdir("dev2");
    cout << "************************" << endl;
    system.cd("dev1");
    system.touch("foo.txt","aaaaaaaa",7);
    cout << "************************" << endl;
    //system.ls();
    system.cd("root");
    cout << "************************" << endl;
    //system.ls();
    cout << "************************" << endl;
    system.ls();
    system.find("foo.txt");
    system.cd("dev1");
    system.mv("foo.txt","dev2/root","-p");
    system.find("foo.txt");
    cout << "************************" << endl;
    system.cd("root");
    system.cd("dev2");
    system.ls();
    system.find("foo.txt");
    cout << "************************" << endl;

}
*/
/*
void main() {
    spthread sp = spthread("Jacy",false,false);
    sp.init_lock("Chenpi");
    sp.init_lock("Eric");
    sp.init_lock("Haoge");
    sp.wrlock("Jacy");
    sp.rdlock("Jacy");

}
*/
void main(){
    GUIcontroller control;
    while (!control.isStop()) {
        control.run();
    }
}
