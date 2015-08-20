#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class Label : public QLabel
{
public:
    explicit Label(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
};

#endif // LABEL_H
