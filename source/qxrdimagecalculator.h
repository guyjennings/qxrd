#ifndef QXRDIMAGECALCULATOR_H
#define QXRDIMAGECALCULATOR_H

#include <QtGui/QDockWidget>

namespace Ui {
    class QxrdImageCalculator;
}

#include "qxrddataprocessor.h"

class QxrdImageCalculator : public QDockWidget {
    Q_OBJECT
public:
    QxrdImageCalculator(QxrdDataProcessorPtr processor, QWidget *parent = 0);
    ~QxrdImageCalculator();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QxrdImageCalculator *m_ui;

    QxrdDataProcessorPtr  m_Processor;
};

#endif // QXRDIMAGECALCULATOR_H
