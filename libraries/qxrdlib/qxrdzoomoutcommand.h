#ifndef QXRDZOOMOUTCOMMAND_H
#define QXRDZOOMOUTCOMMAND_H

#include "qxrdplotbuttoncommand.h"
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>

class QXRD_EXPORT QxrdZoomOutCommand : public QxrdPlotButtonCommand
{
  Q_OBJECT

public:
  QxrdZoomOutCommand(QString                    name,
                     QxrdPlotWidget            *plot,
                     QxrdPlotWidgetSettingsWPtr set);
};

#endif // QXRDZOOMOUTCOMMAND_H
