#ifndef QXRDHISTOGRAMPLOT_H
#define QXRDHISTOGRAMPLOT_H

#include "qcepplot.h"
#include "qxrdhistogramplotsettings.h"

class QxrdHistogramPlot : public QcepPlot
{
  Q_OBJECT
public:
  explicit QxrdHistogramPlot(QWidget *parent = 0);
  void init(QxrdHistogramPlotSettingsWPtr settings);

signals:

public slots:
};

#endif // QXRDHISTOGRAMPLOT_H
