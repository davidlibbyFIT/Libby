#include "mainview.h"
#include "ui_mainview.h"
#include <QtWidgets>
#include <QTcpSocket>

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);
}

MainView::~MainView()
{
    delete ui;
}

QTcpSocket* _pSocket;

void MainView::connectTcp()
{
    QByteArray data; // <-- fill with data
    data = "FCStatus\n\n";

    _pSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * _pSocket;
    connect( _pSocket, SIGNAL(readyRead()), SLOT(readTcpData()) );

    _pSocket->connectToHost("192.168.247.110", 5000);
    if( _pSocket->waitForConnected() ) {
        _pSocket->write( data );
        _pSocket->write( data );
        _pSocket->write( data );
    }
}

void MainView::readTcpData()
{
    QByteArray data = _pSocket->readAll();
    QString output(data);
    ui->lstOutput->addItem(output);

}

void MainView::on_pushButton_clicked()
{
    connectTcp();
}
