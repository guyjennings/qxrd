#ifndef QXRDZOOMINCOMMAND_H
#define QXRDZOOMINCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdZoomInCommand : public QxrdPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdZoomInCommand(QxrdPlotWidget            *plot,
                    QxrdPlotWidgetSettingsWPtr set);

  void enable();
  void disable();
};

#endif // QXRDZOOMINCOMMAND_H
