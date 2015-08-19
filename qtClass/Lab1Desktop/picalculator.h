#ifndef PICALCULATOR_H
#define PICALCULATOR_H

#include <QObject>

class PiCalculator : public QObject
{
    Q_OBJECT
public:
    explicit PiCalculator(QObject *observer = 0);
    ~PiCalculator();

public Q_SLOTS:
    void run();

    void quit();



private:
    QObject *m_observer;
    qreal m_pi;
};

#endif // PICALCULATOR_H
