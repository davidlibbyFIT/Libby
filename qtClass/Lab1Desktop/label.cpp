#include "label.h"
#include <QMouseEvent>

Label::Label(QWidget *parent) :
    QLabel(parent)
{
}

void Label::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        hide();
    QLabel::mousePressEvent(event);
}
