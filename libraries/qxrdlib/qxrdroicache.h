#ifndef QXRDROICACHE_H
#define QXRDROICACHE_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

class QXRD_EXPORT QxrdROICache : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdROICache();

  void redimension(QRect bounds);
  void setPoint(int c, int r, int inner, int outer);
  int  getPoint(int c, int r);

  bool innerPoint(int c, int r);
  bool outerPoint(int c, int r);

  static bool innerPoint(int v);
  static bool outerPoint(int v);

  enum {
    InnerMask = 1,
    OuterMask = 2
  };

private:
  int offsetOf(int c, int r);

  QRect          m_Bounds;
  QVector<qint8> m_Cache;
};

#endif // QXRDROICACHE_H
