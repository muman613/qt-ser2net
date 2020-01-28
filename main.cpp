#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{

    QApplication::setOrganizationName("wunder-bar");
    QApplication::setApplicationName("qt-ser2net");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
