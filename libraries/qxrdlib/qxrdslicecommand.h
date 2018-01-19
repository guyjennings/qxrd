#ifndef QXRDSLICECOMMAND_H
#define QXRDSLICECOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdSliceCommand : public QxrdPlotButtonCommand
{
public:
  QxrdSliceCommand(QString                    name,
                   QxrdPlotWidget            *plot,
                   QxrdPlotWidgetSettingsWPtr set);
};

#endif // QXRDSLICECOMMAND_H
