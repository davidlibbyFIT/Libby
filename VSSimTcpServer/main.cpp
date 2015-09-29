#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    //QCoreApplication a(argc, argv);
    // create MyTcpServer
    // MyTcpServer constructor will create QTcpServer
    //return a.exec();
}
