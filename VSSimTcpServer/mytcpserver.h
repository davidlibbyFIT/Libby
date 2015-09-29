#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    ~MyTcpServer();

signals:

public slots:
    void newConnection();
private:
    QTcpServer *server;

};

#endif // MYTCPSERVER_H
