#ifndef QXRDIMAGECALCULATOR_H
#define QXRDIMAGECALCULATOR_H

#include <QDockWidget>
#include "ui_qxrdimagecalculator.h"

class QxrdDataProcessor;

class QxrdImageCalculator : public QDockWidget, public Ui::QxrdImageCalculator {
    Q_OBJECT
public:
    QxrdImageCalculator(QxrdDataProcessor *processor, QWidget *parent = 0);
    ~QxrdImageCalculator();

protected:
    void changeEvent(QEvent *e);

private:
    QxrdDataProcessor *m_Processor;
};

#endif // QXRDIMAGECALCULATOR_H
