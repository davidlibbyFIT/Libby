#include "mainview.h"
#include "ui_mainview.h"
#include <QtWidgets>
#include <QTcpSocket>

QTcpSocket* _pSocket;
int timerId ;


MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView),
    m_StatusTimer(NULL),
    m_IgnoreDone(false)
{
    ui->setupUi(this);

    QRegExpValidator *v = new QRegExpValidator(this);
    QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
    v->setRegExp(rx);
    ui->lineEdit->setText("192.168.60.142");

    ui->lineEdit->setValidator(v);

    //ui->IpAddress
}

MainView::~MainView()
{
    killTimer(timerId);
    delete ui;
}


void MainView::connectTcp()
{
    QByteArray data; // <-- fill with data
    data = "FCStatus\n\n";

    _pSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * _pSocket;
    connect( _pSocket, SIGNAL(readyRead()), SLOT(readTcpData()) );

    //_pSocket->connectToHost("192.168.247.90", 5000);
    _pSocket->connectToHost(ui->lineEdit->text(), 5000);
    if( _pSocket->waitForConnected() ) {
        //_pSocket->write( data );
        //_pSocket->write( data );
        //_pSocket->write( data );

        //timerId = startTimer(5000);
        m_StatusTimer = new QTimer(this);
        connect(m_StatusTimer, SIGNAL(timeout()), this, SLOT(update()));
        m_StatusTimer->start(5000);

        m_runCount=0;

        //QTimer::singleShot(1000, this, SLOT(update()));
    }
}

void MainView::readTcpData()
{
    QByteArray data = _pSocket->readAll();
    QString output(data);
    ui->lstOutput->addItem(output);

    if(!m_IgnoreDone && output.contains("FCDOne", Qt::CaseInsensitive))
    {
        QTimer::singleShot(1000, this, SLOT(getStats()));
    }

}

void MainView::on_pushButton_clicked()
{
    connectTcp();
}

void MainView::on_pushButton_2_clicked()
{
    QByteArray data; // <-- fill with data
    data = "FCStatus\n\n";

    if( _pSocket->ConnectedState ) {
        //_pSocket->write( data );
        //_pSocket->write( data );
        _pSocket->write( data );

        ui->lstOutput->addItem("<<<< Run Count = " + QString::number(m_runCount) + " >>>>");
        ui->lstOutput->addItem( QDateTime::currentDateTime().toString() + " : " + data);
        ui->lstOutput->scrollToBottom();
    }

}

void MainView::update()
{
    on_pushButton_2_clicked();
}

void MainView::on_pushButton_LaunchRun()
{

}

void MainView::LaunchRun()
{
    QByteArray data; // <-- fill with data
    data = "FCStart\nOutputDirectory=c:\\es\nRunMode=AutoImage\n\n";
    m_runCount++;

    m_StatusTimer->stop();

    if( _pSocket->ConnectedState ) {
        //_pSocket->write( data );
        //_pSocket->write( data );
        _pSocket->write( data );
        ui->lstOutput->addItem( QDateTime::currentDateTime().toString() + " : " + data);
        ui->lstOutput->scrollToBottom();
    }
    m_IgnoreDone=false;
    m_StatusTimer->start(5000);
}

void MainView::on_pushButton_3_clicked()
{
    LaunchRun();

}

void MainView::getStats()
{
    QByteArray data; // <-- fill with data


    data = "FCGetSummary\nGridType=STAT_GRID\nSingleField=Diameter\n\n";



    m_StatusTimer->stop();
    m_IgnoreDone=true;

    if( _pSocket->ConnectedState ) {
        //_pSocket->write( data );
        //_pSocket->write( data );
        _pSocket->write( data );
        ui->lstOutput->addItem( QDateTime::currentDateTime().toString() + " : " + data);
        ui->lstOutput->scrollToBottom();
    }
     m_StatusTimer->start(5000);

    QTimer::singleShot(20000, this, SLOT(LaunchRun()));//300000 == 5 minutes


}
