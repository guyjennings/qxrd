#ifndef QCEPFITTER_H
#define QCEPFITTER_H

#include "qceplib_global.h"
#include "qcepcenterfinder-ptr.h"

class QCEP_EXPORT QcepFitter
{
public:
  QcepFitter(QcepCenterFinder *cf);
  QcepFitter();

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

  QcepCenterFinder *cf() const { return m_CenterFinder; }
  FitResult        reason() const { return m_Reason; }
  QString          reasonString() const;

  static QString   reasonString(FitResult i);
  static QString   reasonString(double v);

protected:
  QcepCenterFinder *m_CenterFinder;
  FitResult         m_Reason;
};

#endif // QCEPFITTER_H
