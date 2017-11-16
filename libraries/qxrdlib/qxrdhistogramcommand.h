#ifndef QXRDHISTOGRAMCOMMAND_H
#define QXRDHISTOGRAMCOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include "qxrdhistogramselector-ptr.h"

class QXRD_EXPORT QxrdHistogramCommand : public QxrdPlotButtonCommand
{
public:
  QxrdHistogramCommand(QString name, QxrdPlotWidget *plot);

  QToolButton *toolButton();

  void enable();
  void disable();

private:
  QxrdHistogramSelector *m_HistogramSelector;
};

#endif // QXRDHISTOGRAMCOMMAND_H
