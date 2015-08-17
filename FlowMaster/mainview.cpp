#include "mainview.h"
#include "ui_mainview.h"
#include <QtWidgets>
#include <QTcpSocket>

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);

    QRegExpValidator *v = new QRegExpValidator(this);
    QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
    v->setRegExp(rx);
    ui->lineEdit->setText("192.168.247.90");

    ui->lineEdit->setValidator(v);

    //ui->IpAddress
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

    //_pSocket->connectToHost("192.168.247.90", 5000);
    _pSocket->connectToHost(ui->lineEdit->text(), 5000);
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
