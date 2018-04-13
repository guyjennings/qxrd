#ifndef QCEPZOOMALLCOMMAND_H
#define QCEPZOOMALLCOMMAND_H

#include "qcepplotbuttoncommand.h"

class QCEP_EXPORT QcepZoomAllCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepZoomAllCommand(QcepPlotWidget            *plot,
                     QcepPlotWidgetSettingsWPtr set);
};

#endif // QCEPZOOMALLCOMMAND_H
