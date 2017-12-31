#ifndef QXRDZOOMOUTCOMMAND_H
#define QXRDZOOMOUTCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdZoomOutCommand : public QxrdPlotButtonCommand
{
public:
  QxrdZoomOutCommand(QString                    name,
                     QxrdPlotWidget            *plot,
                     QxrdPlotWidgetSettingsWPtr set);

  QToolButton *toolButton();
};

#endif // QXRDZOOMOUTCOMMAND_H
