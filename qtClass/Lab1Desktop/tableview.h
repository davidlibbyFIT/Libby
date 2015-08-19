#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QWidget>
#include <QTableView>

class Label;
class QStandardItemModel;
class QResizeEvent;

class TableView : public QTableView
{

    Q_OBJECT

public:
    TableView(QWidget *parent=nullptr);
    ~TableView();

private:
    QStandardItemModel *m_model;
    Label *m_pLabel;
protected:

    void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event)Q_DECL_OVERRIDE;

public slots:

    void clicked(const QModelIndex & index);

};

#endif // TABLEVIEW_H
