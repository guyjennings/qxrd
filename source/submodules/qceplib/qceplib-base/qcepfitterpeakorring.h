#ifndef QCEPFITTERPEAKORRING_H
#define QCEPFITTERPEAKORRING_H

#include "qceplib_global.h"
#include "qcepfitter.h"

class QCEP_EXPORT QcepFitterPeakOrRing : public QcepFitter
{
public:
  QcepFitterPeakOrRing(QcepCenterFinder* cf, int index, double x0, double y0, double pkht, double bkgd);
  QcepFitterPeakOrRing();

  int              index() const { return m_Index; }
  double           x0() const { return m_X0; }
  double           y0() const { return m_Y0; }
  double           pkht() const { return m_Pkht; }
  double           bkgd() const { return m_Bkgd; }
  double           fittedX() const { return m_FittedX; }
  double           fittedY() const { return m_FittedY; }
  double           fittedWidth() const { return m_FittedWidth; }
  double           fittedHeight() const { return m_FittedHeight; }
  double           fittedBkgd() const { return m_FittedBkgd; }
  double           fittedBkgdX() const { return m_FittedBkgdX; }
  double           fittedBkgdY() const { return m_FittedBkgdY; }
  double           fittedR() const;
  double           fittedAz() const;

protected:
  int               m_Index;
  double            m_X0;
  double            m_Y0;
  double            m_Pkht;
  double            m_Bkgd;
  double            m_FittedX;
  double            m_FittedY;
  double            m_FittedWidth;
  double            m_FittedHeight;
  double            m_FittedBkgd;
  double            m_FittedBkgdX;
  double            m_FittedBkgdY;
  double            m_FittedR;
  double            m_FittedAz;
};

#endif // QCEPFITTERPEAKORRING_H
