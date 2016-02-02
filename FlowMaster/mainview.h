#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>

namespace Ui {
class MainView;
}

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

private slots:
    void readTcpData();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void update();
    void getStats();
    void LaunchRun();

    void on_pushButton_LaunchRun();

    void on_pushButton_3_clicked();

private:
    Ui::MainView *ui;
    void connectTcp();

    QTimer *m_StatusTimer;
    bool m_IgnoreDone;
    int m_runCount;
};

#endif // MAINVIEW_H
