#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QPropertyAnimation)
QT_FORWARD_DECLARE_CLASS(QImage)

class CustomWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int angle MEMBER m_angle WRITE setAngle)
public:
    explicit CustomWidget(QWidget *parent = 0);
    void setAngle(int angle);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:
    QPropertyAnimation *m_animation;
    int m_angle;
    QImage m_transparentGradient;
    QPixmap *m_clarice_pix;
};

#endif // CUSTOMWIDGET_H
