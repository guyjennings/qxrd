#ifndef QXRDSLICEPLOT_H
#define QXRDSLICEPLOT_H

#include "qxrdplot.h"

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
