#ifndef QXRDPLOTPREFERENCESBUTTON_H
#define QXRDPLOTPREFERENCESBUTTON_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdPlotPreferencesButton : public QxrdPlotButtonCommand
{
public:
  QxrdPlotPreferencesButton(QString                    name,
                            QxrdPlotWidget            *plot,
                            QxrdPlotWidgetSettingsWPtr set);

  QToolButton *toolButton();
};

#endif // QXRDPLOTPREFERENCESBUTTON_H
