#ifndef QCEPZOOMINCOMMAND_H
#define QCEPZOOMINCOMMAND_H

#include "qcepplotbuttoncommand.h"

class QCEP_EXPORT QcepZoomInCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepZoomInCommand(QcepPlotWidget            *plot,
                    QcepPlotWidgetSettingsWPtr set);

  void enable();
  void disable();
};

#endif // QCEPZOOMINCOMMAND_H
