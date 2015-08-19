#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStandardItemModel"
#include "tableview.h"
#include "customview.h"
#include "picalculator.h"
#include <QThread>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_TableView(new TableView()),
    m_pCustomView(new CustomView()),
    m_pPiCalculator(nullptr),
    m_pPieThread(nullptr)
{
    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
    QWidget::setGeometry(20,20,400,400);

    m_model=new QStandardItemModel(parent);

    m_model->appendRow(new QStandardItem("Cars"));
    m_model->appendRow(new QStandardItem("Temps"));
    m_model->appendRow(new QStandardItem("Style"));

    ui->listView->setModel(m_model);

    ui->stackedWidget->addWidget(m_TableView);
    ui->stackedWidget->addWidget(m_pCustomView);



}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButtonCars_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_TableView);
}

void MainWindow::on_pushButtonTemps_clicked()
{

}
void MainWindow::on_pushButtonStyle_clicked()
{
    ui->stackedWidget->setCurrentWidget(m_pCustomView);
}

void MainWindow::PieCalcFinished()
{
    delete m_pPieThread;
    delete m_pPiCalculator;
    m_pPieThread =nullptr;
    m_pPieThread =nullptr;
    ui->pushButtonStartTask->setText("Start Thread");
}
void MainWindow::on_pushButtonStartTask_clicked()
{
    if(!m_pPieThread&&!m_pPiCalculator)
    {
        m_pPieThread=new QThread(this);
        PiCalculator *m_pPiCalculator=new PiCalculator(this);
        m_pPiCalculator->moveToThread(m_pPieThread);
        connect(m_pPieThread,QThread::started,m_pPiCalculator,&PiCalculator::run);
        connect(m_pPieThread,QThread::finished,this,&MainWindow::PieCalcFinished);
        m_pPieThread->start();
        ui->pushButtonStartTask->setText("Stop Thread");
    }else
    {
        m_pPieThread->requestInterruption();
    }
}

void MainWindow::handleResults()
{

}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{


    switch (index.row())
    {
    case 0:
        on_pushButtonCars_clicked();
        break;
    case 1:
        on_pushButtonTemps_clicked();
        break;
    case 2:
        on_pushButtonTemps_clicked();
        break;
    default:
        break;
    }
}
