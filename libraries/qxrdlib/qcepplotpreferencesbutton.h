#ifndef QCEPPLOTPREFERENCESBUTTON_H
#define QCEPPLOTPREFERENCESBUTTON_H

#include "qcepplotbuttoncommand.h"

class QXRD_EXPORT QcepPlotPreferencesButton : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepPlotPreferencesButton(QcepPlotWidget            *plot,
                            QcepPlotWidgetSettingsWPtr set);
};

#endif // QCEPPLOTPREFERENCESBUTTON_H
