#ifndef BOOTANIMATION_H
#define BOOTANIMATION_H
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

    MainWindow window;
    window.show();

    screen.finish(&window);

    return app.exec();
}


