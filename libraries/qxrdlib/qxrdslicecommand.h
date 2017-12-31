#ifndef QXRDSLICECOMMAND_H
#define QXRDSLICECOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdSliceCommand : public QxrdPlotButtonCommand
{
public:
  QxrdSliceCommand(QString                    name,
                   QxrdPlotWidget            *plot,
                   QxrdPlotWidgetSettingsWPtr set);

  QToolButton *toolButton();
};

#endif // QXRDSLICECOMMAND_H
