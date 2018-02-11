#ifndef QXRDPLOTPREFERENCESBUTTON_H
#define QXRDPLOTPREFERENCESBUTTON_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdPlotPreferencesButton : public QxrdPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdPlotPreferencesButton(QxrdPlotWidget            *plot,
                            QxrdPlotWidgetSettingsWPtr set);
};

#endif // QXRDPLOTPREFERENCESBUTTON_H
