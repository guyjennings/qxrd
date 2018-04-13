#ifndef QCEPZOOMOUTCOMMAND_H
#define QCEPZOOMOUTCOMMAND_H

#include "qcepplotbuttoncommand.h"
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>

class QCEP_EXPORT QcepZoomOutCommand : public QcepPlotButtonCommand
{
  Q_OBJECT

public:
  QcepZoomOutCommand(QcepPlotWidget            *plot,
                     QcepPlotWidgetSettingsWPtr set);
};

#endif // QCEPZOOMOUTCOMMAND_H
