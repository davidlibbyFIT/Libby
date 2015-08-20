#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QWidget>
#include <QTableView>

class Label;
class QStandardItemModel;

class TableView : public QTableView
{

    Q_OBJECT

public:
    TableView(QWidget *parent=nullptr);
    ~TableView();

private:
    QStandardItemModel *m_model;
    Label *m_pLabel;

public slots:

    void clicked(const QModelIndex & index);

};

#endif // TABLEVIEW_H
