#ifndef QXRDFITTER_H
#define QXRDFITTER_H

#include "qxrdlib_global.h"
#include "qxrdcenterfinder-ptr.h"

class QXRD_EXPORT QxrdFitter
{
public:
  QxrdFitter(QxrdCenterFinder* cf);
  QxrdFitter();

  enum FitResult {
    Successful,
    NoResult,
    OutsideData,
    BadWidth,
    BadPosition,
    BadHeight,
    LastReason
  };

  virtual int     fit() = 0;

  QxrdCenterFinder *cf() const { return m_CenterFinder; }
  FitResult        reason() const { return m_Reason; }
  QString          reasonString() const;

  static QString   reasonString(FitResult i);
  static QString   reasonString(double v);

protected:
  QxrdCenterFinder *m_CenterFinder;
  FitResult         m_Reason;
};

#endif // QXRDFITTER_H
