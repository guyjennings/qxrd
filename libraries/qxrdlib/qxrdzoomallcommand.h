#ifndef QXRDZOOMALLCOMMAND_H
#define QXRDZOOMALLCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdZoomAllCommand : public QxrdPlotButtonCommand
{
public:
  QxrdZoomAllCommand(QString                    name,
                     QxrdPlotWidget            *plot,
                     QxrdPlotWidgetSettingsWPtr set);
};

#endif // QXRDZOOMALLCOMMAND_H