#include "guicontroller.h"


using namespace std;

int main() {
    GUIcontroller control("Jacy");
    while (!control.isStop()) {
        control.update();
    }
    return 0;
}




