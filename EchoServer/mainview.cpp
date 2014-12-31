#include "mainview.h"
#include "ui_mainview.h"
#include <QTcpServer>
#include <QMessageBox>
#include <QTcpSocket>
#include <QScrollBar>
#include <QNetworkInterface>

static int PORT_NUMBER = 9999;
static int WAIT_FOR_DATA_MS = 200;

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView),
    m_pServer(new QTcpServer(this)),
    m_pSocket(nullptr)
{
    ui->setupUi(this);
    ui->btnStop->setEnabled(false);
    connect(m_pServer,&QTcpServer::newConnection,this,&MainView::ExchangeData);

#ifdef ANDROID
    ui->lstResult->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 100px; }");
    ui->lstIp->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 100px; }");
    ui->lstIp->setMinimumHeight(500);
    //QString mystyle = tr("QScrollBar:vertical { width: %1px; }").arg(100);
     //ui->txtBox->verticalScrollBar()->setStyleSheet(mystyle);


#endif

}

MainView::~MainView()
{
    delete ui;
}

void MainView::on_btnStart_clicked()
{
    if(StartServer())
    {
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);

        QList<QHostAddress> list = QNetworkInterface::allAddresses();

         for(int nIter=0; nIter<list.count(); nIter++)

          {
              if(!list[nIter].isLoopback())
                  if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
                qDebug() << list[nIter].toString();
              QString IpEtc=list[nIter].toString();
              IpEtc+=":";
              IpEtc+=QString::number(PORT_NUMBER);

              ui->lstIp->addItem(IpEtc);

          }


    }
    ui->lstResult->clear();
}

void MainView::on_btnStop_clicked()
{
    StopServer();
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->lstIp->clear();
}

bool MainView::StartServer()
{
    bool result = m_pServer->listen(QHostAddress::Any,PORT_NUMBER);
    if(!result)
    {
        QMessageBox::critical(this,tr("Echo Server"),
                              tr("Unable to start the server: %1")
                              .arg(m_pServer->errorString()));
        return false;
    }
    return true;
}

void MainView::StopServer()
{
    m_pServer->close();
    if(m_pSocket!= nullptr && m_pSocket->isOpen())
    {
        m_pSocket->close();
    }
}

void MainView::ExchangeData()
{
    m_pSocket = m_pServer->nextPendingConnection();
    if(m_pSocket->isOpen())
    {
        connect(m_pSocket,&QTcpSocket::readyRead,
                this,&MainView::EchoReadData);
    }
}
void MainView::EchoReadData()
{
    m_pSocket->write("<EchoServer>\n");
    QString result;
    while (!m_pSocket->atEnd())
    {
        result.append(m_pSocket->readAll());
        m_pSocket->waitForReadyRead(WAIT_FOR_DATA_MS);

    }
    m_pSocket->write(qPrintable(result));
    m_pSocket->write("\n</EchoServer>\n");
    ui->lstResult->addItem(result);
}
