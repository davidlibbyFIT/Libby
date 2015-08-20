#include "tableview.h"
#include <QStandardItemModel>
#include <QDebug>
#include "label.h"
#include <QPixmap>

TableView::TableView(QWidget *parent):
    m_pLabel(nullptr)

{
    QList <QStringList> carData;
    carData.append(QStringList( { { "Toyota" }, { "Avensis" } } ));
    carData.append(QStringList( { { "Toyota" }, { "Land-Cruiser" } } ));
    carData.append(QStringList( { { "Ford" }, { "Galaxie" } } ));
    carData.append(QStringList( { { "Ford" }, { "Ka" } } ));
    carData.append(QStringList( { { "Ford" }, { "Mustang" } } ));
    carData.append(QStringList( { { "Ford" }, { "Escort" } } ));
    carData.append(QStringList( { { "Chevrolet" }, { "Camaro" } } ));
    carData.append(QStringList( { { "Chevrolet" }, { "Corvette" } } ));
    carData.append(QStringList( { { "Jaguar" }, { "E-Type" } } ));
    carData.append(QStringList( { { "Jaguar" }, { "X-Type" } } ));
    carData.append(QStringList( { { "Nissan" }, { "Qashqai" } } ));

    connect(this,&QTableView::clicked,this,&TableView::clicked);

    QStandardItemModel *m_model=new QStandardItemModel(this);

    int rowct=0;

    m_model->insertRows(0,carData.count());
    m_model->insertColumns(0,2);
    Q_FOREACH (const QStringList &singlerow, carData)
    {
        m_model->setData(m_model->index(rowct,0),singlerow.at(0));
        m_model->setData(m_model->index(rowct,1),singlerow.at(1));
        rowct++;
    }
     //m_model->setHorizontalHeaderItem(0,"car");

    //m_model->setHeaderData(0,"car");
   // m_model->setHeaderData(0,"Model");
    setSortingEnabled(true);
    setModel(m_model);

    m_pLabel = new Label();
    m_pLabel->setGeometry(0,0,600,600);
    m_pLabel->setWindowFlags( Qt::FramelessWindowHint);

}
TableView::~TableView()
{
    delete m_pLabel;
}
void TableView::clicked(const QModelIndex & index)
{
    QVariant vdata=index.data();
    int row=index.row();

    if(!vdata.canConvert(QMetaType::QString))
        return;
    QString sdata=vdata.toString();

    //QStandardItemModel *mod=qobject_cast<QStandardItemModel>
    QString man = model()->data(model()->index(row,0)).toString();
    QString mod = model()->data(model()->index(row,1)).toString();

    //qDebug() << man << " : " << mod ;

    QString CarName= ":/Images/"+man+"-"+""+mod+".jpg";

    CarName = CarName.toLower();

    qDebug() << CarName;

    //QPixmap MyPick(QPixmap(CarName).scaled(600,600));
    //QPixmap * mypix = new QPixmap(CarName);
    QPixmap mypix;
    mypix.load(CarName);
    m_pLabel->setPixmap(mypix);
    m_pLabel->show();


}
