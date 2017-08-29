#ifndef QXRDEXPOSURETIMESPINNER_H
#define QXRDEXPOSURETIMESPINNER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include <QDoubleSpinBox>
#include <QVector>

class QXRD_EXPORT QxrdExposureTimeSpinner : public QDoubleSpinBox
{
  Q_OBJECT
public:
  explicit QxrdExposureTimeSpinner(QWidget *parent = 0);

  virtual void stepBy(int steps);
signals:

public slots:

private:
  QVector<double> m_PreferredExposures;
};

#endif // QXRDEXPOSURETIMESPINNER_H
