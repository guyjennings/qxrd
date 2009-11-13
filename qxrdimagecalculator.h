#ifndef QXRDIMAGECALCULATOR_H
#define QXRDIMAGECALCULATOR_H

#include <QtGui/QDockWidget>

namespace Ui {
    class QxrdImageCalculator;
}

class QxrdDataProcessor;

class QxrdImageCalculator : public QDockWidget {
    Q_OBJECT
public:
    QxrdImageCalculator(QxrdDataProcessor *processor, QWidget *parent = 0);
    ~QxrdImageCalculator();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QxrdImageCalculator *m_ui;

    QxrdDataProcessor *m_Processor;
};

#endif // QXRDIMAGECALCULATOR_H
