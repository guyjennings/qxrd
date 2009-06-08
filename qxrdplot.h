#ifndef QXRDPLOT_H
#define QXRDPLOT_H

#include <qwt_plot.h>

class QxrdPlot : public QwtPlot
{
  Q_OBJECT;
public:
  QxrdPlot(QWidget *parent = 0);

public slots:
  void autoScalePlot();
};

#endif // QXRDPLOT_H
