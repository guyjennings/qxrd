#ifndef QCEPPRINTPLOTBUTTON_H
#define QCEPPRINTPLOTBUTTON_H

#include "qcepplotbuttoncommand.h"

class QCEP_EXPORT QcepPrintPlotButton : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepPrintPlotButton(QcepPlotWidget            *plot,
                      QcepPlotWidgetSettingsWPtr set);
};

#endif // QCEPPRINTPLOTBUTTON_H
