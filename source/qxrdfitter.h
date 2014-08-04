#ifndef QXRDFITTER_H
#define QXRDFITTER_H

#include "qxrdcenterfinder-ptr.h"

class QxrdFitter
{
public:
  QxrdFitter(QxrdCenterFinder* cf);
  QxrdFitter();

  enum FitResult {
    NoResult,
    OutsideData,
    Successful,
    BadWidth,
    BadPosition,
    BadHeight
  };

  virtual int     fit() = 0;

  QxrdCenterFinder *cf() const { return m_CenterFinder; }
  FitResult        reason() const { return m_Reason; }
  QString          reasonString() const;

protected:
  QxrdCenterFinder *m_CenterFinder;
  FitResult         m_Reason;
};

#endif // QXRDFITTER_H
