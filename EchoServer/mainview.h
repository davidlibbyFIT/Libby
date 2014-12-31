#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>

namespace Ui {
class MainView;
}

class QTcpServer;
class QTcpSocket;


class MainView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();

private:
    Ui::MainView *ui;
    bool StartServer();
    void StopServer();
    QTcpServer* m_pServer;
    QTcpSocket* m_pSocket;

    void ExchangeData();
    void EchoReadData();
};

#endif // MAINVIEW_H
