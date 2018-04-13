#ifndef QCEPHISTOGRAMCOMMAND_H
#define QCEPHISTOGRAMCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include "qcephistogramselector-ptr.h"

class QCEP_EXPORT QcepHistogramCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepHistogramCommand(QcepPlotWidget            *plot,
                       QcepPlotWidgetSettingsWPtr set);

  void enable();
  void disable();

private:
  QcepHistogramSelector *m_HistogramSelector;
};

#endif // QCEPHISTOGRAMCOMMAND_H
