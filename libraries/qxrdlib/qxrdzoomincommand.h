#ifndef QXRDZOOMINCOMMAND_H
#define QXRDZOOMINCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdZoomInCommand : public QxrdPlotButtonCommand
{
public:
  QxrdZoomInCommand(QString                    name,
                    QxrdPlotWidget            *plot,
                    QxrdPlotWidgetSettingsWPtr set);

  void enable();
  void disable();
};

#endif // QXRDZOOMINCOMMAND_H