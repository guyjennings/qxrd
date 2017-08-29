#ifndef QXRDFITTEDRINGS_H
#define QXRDFITTEDRINGS_H

#include "qcepdatacolumnscan.h"
#include "qxrdfittedrings-ptr.h"

class QxrdFittedRings : public QcepDataColumnScan
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdFittedRings(QString name, int sz=0);

  static QxrdFittedRingsPtr newFittedRings(QString name);

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

#endif // QXRDFITTEDRINGS_H
