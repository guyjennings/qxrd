#ifndef QXRDFITTER_H
#define QXRDFITTER_H

#include "qxrdcenterfinder-ptr.h"

class QxrdFitter
{
public:
  QxrdFitter(QxrdCenterFinder* cf, int index, double x0, double y0, double pkht, double bkgd);
  QxrdFitter();

  enum FitResult {
    NoResult,
    OutsideData,
    Successful,
    BadWidth,
    BadPosition,
    BadHeight
  };

  virtual void     fit() = 0;

  QxrdCenterFinder *cf() const { return m_CenterFinder; }
  int              index() const { return m_Index; }
  double           x0() const { return m_X0; }
  double           y0() const { return m_Y0; }
  double           pkht() const { return m_Pkht; }
  double           bkgd() const { return m_Bkgd; }
  FitResult        reason() const { return m_Reason; }
  QString          reasonString() const;
  double           fittedX() const { return m_FittedX; }
  double           fittedY() const { return m_FittedY; }
  double           fittedWidth() const { return m_FittedWidth; }
  double           fittedHeight() const { return m_FittedHeight; }
  double           fittedBkgd() const { return m_FittedBkgd; }
  double           fittedBkgdX() const { return m_FittedBkgdX; }
  double           fittedBkgdY() const { return m_FittedBkgdY; }

protected:
  QxrdCenterFinder *m_CenterFinder;
  int               m_Index;
  double            m_X0;
  double            m_Y0;
  double            m_Pkht;
  double            m_Bkgd;
  FitResult         m_Reason;
  double            m_FittedX;
  double            m_FittedY;
  double            m_FittedWidth;
  double            m_FittedHeight;
  double            m_FittedBkgd;
  double            m_FittedBkgdX;
  double            m_FittedBkgdY;
};

#endif // QXRDFITTER_H
