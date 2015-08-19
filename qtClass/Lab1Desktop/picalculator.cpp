#include "picalculator.h"
#include <float.h>
#include <QtMath>
#include <QThread>
#include <QProgressBar>
#include <QAbstractEventDispatcher>
#include <QTimer>
#include <QDebug>

#define TIMER_DURATION 1000*30 //30 seconds...

PiCalculator::PiCalculator(QObject *observer) :
    m_observer(observer),
    m_pi(1.0)
{
}

PiCalculator::~PiCalculator()
{
    qDebug() << "Pi Calculator destructed";
}

void PiCalculator::run()
{
    QTimer *m_timer = new QTimer(this);
    m_timer->start(TIMER_DURATION);
    connect(m_timer, &QTimer::timeout, this, &PiCalculator::quit);
    qreal k(3.0);
    bool minus(true);

    Q_FOREVER {
        // CHECK FOR INTERRUPTION
        if(thread()->isInterruptionRequested())
        {
            thread()->quit();
            break;

        }
        m_pi += minus ? -1/k : 1/k;
        k += 2;
        minus = minus ? false : true;
       qDebug() << "Pi =" << m_pi * 4;
        if (qFloor(k) % 999 == 0) {
            // NOTIFY THE PROGRESS BAR (total time - remaining time) / total time
        }
        thread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);

        //QMetaObject::invokeMethod(m_observer,"UpdateGuage",);

    }

}

void PiCalculator::quit()
{
    thread()->requestInterruption();
}


