#include "customview.h"
#include "customwidget.h"
#include <QPushButton>
#include <QVBoxLayout>

CustomView::CustomView(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QPushButton *button = new QPushButton(tr("Press me"));
    layout->addWidget(button);

    //button->setFlat(true);
    button->setStyleSheet("QPushButton {background-color:red;}");

    //QWidget::setStyleSheet("CustomView.QPushButton{qproperty-flat:true;}");

    CustomWidget *widget = new CustomWidget(this);
    layout->addWidget(widget);
}
