#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStandardItemModel;
class TableView;
class CustomView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonCars_clicked();

    void on_pushButtonTemps_clicked();

    void on_pushButtonStyle_clicked();

    void on_pushButtonStartTask_clicked();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_model;
    TableView *m_TableView;
    CustomView *m_pCustomView;
};

#endif // MAINWINDOW_H
