#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, 5000))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

MyTcpServer::~MyTcpServer()
{

}

void MyTcpServer::newConnection()
{
    // need to grab the socket
    QTcpSocket *socket = server->nextPendingConnection();

    socket->write("Hello client\r\n");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}
