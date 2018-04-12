#ifndef QCEPPLOTPREFERENCESBUTTON_H
#define QCEPPLOTPREFERENCESBUTTON_H

#include "qcepplotbuttoncommand.h"

class QCEP_EXPORT QcepPlotPreferencesButton : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepPlotPreferencesButton(QcepPlotWidget            *plot,
                            QcepPlotWidgetSettingsWPtr set);
};

#endif // QCEPPLOTPREFERENCESBUTTON_H
