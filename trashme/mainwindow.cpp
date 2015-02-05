#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <map>
#include <vector>


typedef std::map<std::string, std::string> TRowMap;
typedef TRowMap::iterator iTRowMap;

typedef std::vector<TRowMap> TDataVector;
typedef TDataVector::iterator iTDataVector;

TDataVector m_Gold;
TDataVector m_RunData;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

   {
        TRowMap GoldRow;
        GoldRow["First Name"]="David";
        GoldRow["Last Name"]="Libby";
        m_Gold.push_back(GoldRow);
   }

    {
         TRowMap GoldRow;
         GoldRow["First Name"]="Mike";
         GoldRow["Last Name"]="Roe";
         m_Gold.push_back(GoldRow);
    }

    for (iTDataVector ittr=m_Gold.begin();ittr != m_Gold.end();ittr++)
    {
        int aa=1;
    }

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
