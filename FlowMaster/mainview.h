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

private:
    Ui::MainView *ui;
    void connectTcp();
};

#endif // MAINVIEW_H
