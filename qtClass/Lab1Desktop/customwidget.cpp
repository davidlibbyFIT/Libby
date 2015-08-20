#include "customwidget.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QApplication>
#include <QStyleOption>

CustomWidget::CustomWidget(QWidget *parent) :
    QWidget(parent), m_animation(0), m_angle(0), m_transparentGradient(0)
{
    m_clarice_pix = new QPixmap(":/images/clarice_the_bear.jpg");

}

void CustomWidget::setAngle(int angle)
{
    if (m_angle != angle) {
        m_angle = angle;
        update();
    }
}

void CustomWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    // Draw ellipse with radial gradient
    QRadialGradient g(60, 240, 50);
    g.setColorAt(0.0, Qt::yellow);
    g.setColorAt(1.0, Qt::black);
    g.setSpread(QGradient::RepeatSpread);
    p.setBrush(g);
    p.drawEllipse(20, 200, 80, 80);

    // Draw an image and its reflection
    QImage image(":/images/clarice_the_bear.jpg");




    p.save();
    p.rotate(m_angle);
    p.drawPixmap(0,0,*m_clarice_pix);
    //p.drawImage(0, 0, image);
    p.restore();

    int imageHeight(image.height());
    int imageWidth(image.width());
    p.drawImage(200, 0, image);

    if(m_transparentGradient.isNull())
    {
        m_transparentGradient = QImage(imageWidth, imageHeight, QImage::Format_ARGB32_Premultiplied);
        QPainter imagePainter(&m_transparentGradient);
        QLinearGradient lG2(imageWidth/2, 0, imageWidth/2, imageHeight);
        lG2.setColorAt(0.0, QColor(0, 0, 0, 255));
        lG2.setColorAt(1.0, QColor(0, 0, 0, 0));
        imagePainter.setBrush(lG2);
        imagePainter.setPen(Qt::NoPen);
        imagePainter.drawRect(0, 0, imageWidth, imageHeight);

        imagePainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        imagePainter.drawImage(0, 0, image);

        imagePainter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
        imagePainter.drawImage(0, 0, image);
    }

    
    QTransform t;
    t.rotate(180);
    t.rotate(180, Qt::YAxis);
    p.setTransform(t);
    p.drawImage(200, - (2 * imageHeight), m_transparentGradient);
}

void CustomWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (m_animation) {
        if (m_animation->state() == QAbstractAnimation::Running)
            return;
        else
            delete m_animation;
    }


    QWidget::setStyleSheet("CustomWidget{qproperty-angle:45;}");
    //QWidget::setStyleSheet("CustomWidget{qproperty-angle:-90;}");

    m_animation = new QPropertyAnimation(this, "angle", this);
    m_animation->setDuration(2000);
    m_animation->setStartValue(0);
    m_animation->setEndValue(property("angle"));
    m_animation->start();
}
