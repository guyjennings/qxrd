#ifndef QXRDSLICEPLOT_H
#define QXRDSLICEPLOT_H

#include "qxrdplot.h"
#include "qxrdplotsettings.h"

class QxrdSlicePlot : public QxrdPlot
{
  Q_OBJECT
public:
  explicit QxrdSlicePlot(QWidget *parent = 0);
  void init(QxrdPlotSettingsWPtr settings);

signals:

public slots:

};

#endif // QXRDSLICEPLOT_H
