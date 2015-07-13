#ifndef QXRDSLICEPLOT_H
#define QXRDSLICEPLOT_H

#include "qcepplot.h"
#include "qcepplotsettings-ptr.h"

class QxrdSlicePlot : public QcepPlot
{
  Q_OBJECT
public:
  explicit QxrdSlicePlot(QWidget *parent = 0);
  void init(QcepPlotSettingsWPtr settings);

signals:

public slots:

};

#endif // QXRDSLICEPLOT_H
