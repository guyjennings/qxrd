#ifndef QXRDHISTOGRAMPLOT_H
#define QXRDHISTOGRAMPLOT_H

#include "qxrdlib_global.h"
#include "qcepplot.h"
#include "qxrdhistogramplotsettings-ptr.h"

class QXRD_EXPORT QxrdHistogramPlot : public QcepPlot
{
  Q_OBJECT
public:
  explicit QxrdHistogramPlot(QWidget *parent = 0);
  void init(QxrdHistogramPlotSettingsWPtr settings);

signals:

public slots:
};

#endif // QXRDHISTOGRAMPLOT_H
