#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MyTcpServer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    MyTcpServer *m_pServer;
};

#endif // MAINWINDOW_H
