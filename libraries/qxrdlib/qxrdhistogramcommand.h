#ifndef QXRDHISTOGRAMCOMMAND_H
#define QXRDHISTOGRAMCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qxrdhistogramselector-ptr.h"

class QXRD_EXPORT QxrdHistogramCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdHistogramCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  void enable();
  void disable();

private:
  QxrdHistogramSelector *m_HistogramSelector;
};

#endif // QXRDHISTOGRAMCOMMAND_H
