#ifndef QCEPFITTEDRINGS_H
#define QCEPFITTEDRINGS_H

#include "qceplib_global.h"
#include "qcepdatacolumnscan.h"
#include "qcepfittedrings-ptr.h"

class QCEP_EXPORT QcepFittedRings : public QcepDataColumnScan
{
  Q_OBJECT

private:
  typedef QcepDataColumnScan inherited;

public:
  Q_INVOKABLE QcepFittedRings(QString name, int sz=0);
  void initialize(QcepObjectWPtr parent);

public slots:
  void append(int n,
              double x,
              double y,
              double a,
              double b,
              double rot);

  int    n(int i);
  double x(int i);
  double y(int i);
  double a(int i);
  double b(int i);
  double rot(int i);
};

#endif // QCEPFITTEDRINGS_H
