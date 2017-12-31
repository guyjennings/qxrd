#ifndef QXRDMASKCIRCLESCOMMAND_H
#define QXRDMASKCIRCLESCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdMaskCirclesCommand : public QxrdPlotButtonCommand
{
public:
  QxrdMaskCirclesCommand(QString                    name,
                         QxrdPlotWidget            *plot,
                         QxrdPlotWidgetSettingsWPtr set);

  QToolButton *toolButton();
};

#endif // QXRDMASKCIRCLESCOMMAND_H
