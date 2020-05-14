#include "sFileSystem.h"
#include <string>
#include <iostream>
#include <error.h>
#include "console.h"

void main() {
    sFileSystem system = sFileSystem("Jacy");
    cout << "************************" << endl;
    system.pwd();
    system.touch("foo.txt","foo",7);
    system.touch("foo2.txt","Hello world",4);
    system.touch("hello.txt","NoNoNo.",7);
    system.mkdir("dev1");
    system.mkdir("dev2");
    cout << "************************" << endl;
    system.ls();
    system.cd("dev1");
    system.touch("foo.txt","aaaaaaaa",7);
    cout << "************************" << endl;
    system.ls();
    system.cd("root");
    cout << "************************" << endl;
    system.ls();
    system.rm("foo.txt");
    cout << "************************" << endl;
    system.ls();
    system.file_info("foo2.txt");
    system.find("foo.txt");
    system.cd("dev1");
    system.mv("foo.txt","dev2/root","-p");
    cout << "************************" << endl;
    system.cd("root");
    system.cd("dev2");
    system.ls();
    system.find("foo.txt");
    cout << "************************" << endl;

}
