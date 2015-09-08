#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QQuickView>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    QQuickView *view = new QQuickView();
    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setMinimumSize(400,400);
    container->setMaximumSize(400,400);
    container->setFocusPolicy(Qt::TabFocus);



    //QObject *object = view->rootObject();
    connect(view,SIGNAL(mySignal()),this,SLOT(cppSlot()));


    view->setSource(QUrl("http://fitdb/splitcolor.qml"));
    view->setSource(QUrl("qrc:/qml/test.qml"));

    ui->verticalLayout->addWidget(container);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::cppSlot()
{
    qDebug() << "Called the C++ slot with";
}
