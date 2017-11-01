#ifndef QXRDIMAGECALCULATOR_H
#define QXRDIMAGECALCULATOR_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "ui_qxrdimagecalculator.h"
#include "qxrddataprocessor-ptr.h"

class QXRD_EXPORT QxrdImageCalculator : public QDockWidget, public Ui::QxrdImageCalculator {
    Q_OBJECT
public:
    QxrdImageCalculator(QxrdDataProcessorWPtr processor, QWidget *parent);
    ~QxrdImageCalculator();

protected:
    void changeEvent(QEvent *e);

private:
    QxrdDataProcessorWPtr m_Processor;
};

#endif // QXRDIMAGECALCULATOR_H