#ifndef QXRDPRINTPLOTBUTTON_H
#define QXRDPRINTPLOTBUTTON_H

#include "qxrdplotbuttoncommand.h"

class QXRD_EXPORT QxrdPrintPlotButton : public QxrdPlotButtonCommand
{
public:
  QxrdPrintPlotButton(QString                    name,
                      QxrdPlotWidget            *plot,
                      QxrdPlotWidgetSettingsWPtr set);

  QToolButton *toolButton();
};

#endif // QXRDPRINTPLOTBUTTON_H
