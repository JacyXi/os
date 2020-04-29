#include "bootanimation.h"

#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <mainwindow.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPixmap pixmap("screen.png");
    QSplashScreen screen(pixmap);
    screen.show();
    screen.showMessage("Start!", Qt::AlignCenter, Qt::blue);
    if (0){
        int TotalTime = 6;
        QElapsedTimer timer;
        timer.start();
        if(timer.elapsed() < (TotalTime * 1000)){

             app.processEvents();
        }


    }


    if (0){
        QDateTime n=QDateTime::currentDateTime();
        QDateTime now;
        //一共出现多长时间
         while (n.secsTo(now)<=6){
             now=QDateTime::currentDateTime();
             app.processEvents();
         }

    }

    MainWindow window;
    window.show();

    screen.finish(&window);

    return app.exec();
}


