#ifndef QXRDCENTERSTEPSPINNER_H
#define QXRDCENTERSTEPSPINNER_H

#include "qcepmacros.h"
#include <QDoubleSpinBox>

class QxrdCenterStepSpinner : public QDoubleSpinBox
{
  Q_OBJECT
public:
  explicit QxrdCenterStepSpinner(QWidget *parent = 0);

  virtual void stepBy(int steps);

signals:
public slots:
};

#endif // QXRDCENTERSTEPSPINNER_H
