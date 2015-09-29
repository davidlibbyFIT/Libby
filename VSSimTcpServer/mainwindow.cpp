#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mytcpserver.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pServer = new MyTcpServer();

}

MainWindow::~MainWindow()
{
    delete ui;
}
