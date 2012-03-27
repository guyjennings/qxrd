#ifndef QXRDHISTOGRAMPLOT_H
#define QXRDHISTOGRAMPLOT_H

#include "qxrdplot.h"
#include "qxrdhistogramplotsettings.h"

class QxrdHistogramPlot : public QxrdPlot
{
  Q_OBJECT
public:
  explicit QxrdHistogramPlot(QWidget *parent = 0);
  void init(QxrdHistogramPlotSettingsWPtr settings);

signals:

public slots:
};

#endif // QXRDHISTOGRAMPLOT_H
