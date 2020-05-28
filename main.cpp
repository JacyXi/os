#include "sFileSystem.h"
#include <string>
#include <iostream>
#include <error.h>
#include "spthread.h"
#include <gwindow.h>
#include <gbutton.h>
#include <gtextfield.h>
#include "guicontroller.h"
#include "Calculator.h"

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
    system.mv("foo.txt","dev1/root","-p");
    cout << system.ls();
    system.cd("dev1");
    cout << system.ls();


}*/

/*
void main() {
    spthread sp = spthread("Jacy",false,false);
    sp.init_lock("Chenpi");
    sp.init_lock("Eric");
    sp.init_lock("Haoge");
    sp.rdlock("haoge");
    sp.wrlock("Jacy");
    sp.unlock("haoge");
    sp.rdlock("Jacy");

}
*/
/*
void main() {
    string a;
    cout<<"Please enter the expressions you want to calculate:"<<endl;
    getline(cin,a);
    Calculator *cal = new Calculator();
    cout << cal->run(a);
}
*/


int main() {
    GUIcontroller control("Jacy");
    control.update();
    return 0;
}


/*
void main(){
    GWindow gw;
    GChooser * ch = new  GChooser();
    ch->addItem("Jacy");
    ch->addItem("Eric");
    ch->addItem("UUU");
    ch->setSelectedItem("Eric");
    gw.add(ch);
    gw.pause(100000000);

}

*/

