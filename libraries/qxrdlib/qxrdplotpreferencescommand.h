#ifndef QXRDPLOTPREFERENCESCOMMAND_H
#define QXRDPLOTPREFERENCESCOMMAND_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdPlotPreferencesCommand : public QxrdPlotButtonCommand
{
public:
  QxrdPlotPreferencesCommand(QString                    name,
                             QxrdPlotWidget            *plot,
                             QxrdPlotWidgetSettingsWPtr set);

  QToolButton *toolButton();
};

#endif // QXRDPLOTPREFERENCESCOMMAND_H
