#ifndef QXRDSLICEPLOT_H
#define QXRDSLICEPLOT_H

#include "qxrdlib_global.h"
#include "qcepplot.h"
#include "qcepplotsettings-ptr.h"

class QXRD_EXPORT QxrdSlicePlot : public QcepPlot
{
  Q_OBJECT
public:
  explicit QxrdSlicePlot(QWidget *parent = 0);
  void init(QcepPlotSettingsWPtr settings);

signals:

public slots:

};

#endif // QXRDSLICEPLOT_H
