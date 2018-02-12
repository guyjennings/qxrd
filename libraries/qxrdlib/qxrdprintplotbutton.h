#ifndef QXRDPRINTPLOTBUTTON_H
#define QXRDPRINTPLOTBUTTON_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdPrintPlotButton : public QxrdPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdPrintPlotButton(QxrdPlotWidget            *plot,
                      QxrdPlotWidgetSettingsWPtr set);
};

#endif // QXRDPRINTPLOTBUTTON_H