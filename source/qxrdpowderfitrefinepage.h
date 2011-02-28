#ifndef QXRDPOWDERFITREFINEPAGE_H
#define QXRDPOWDERFITREFINEPAGE_H

#include "qxrdpowderfitidentifypage.h"

class QxrdPowderFitRefinePage : public QxrdPowderFitIdentifyPage
{
  Q_OBJECT;

public:
  QxrdPowderFitRefinePage(QxrdDataProcessor *proc, QWidget *parent);

public slots:
  void selectRefinePage();
  void appendRingFit();
  void removeRingFit();

protected:
  void appendFitParameter(int n, QxrdFitParameter *parm);
  void appendStaticFitParameter(QxrdFitParameter *parm);
  void appendRingFitParameter(int n, QxrdFitParameter *parm);
  void removeRingFitParameter(int n);

private:
  QxrdRingSetFitParametersPtr m_RingSet;
  int                         m_NStatic;
  int                         m_NRings;
};

#endif // QXRDPOWDERFITREFINEPAGE_H
