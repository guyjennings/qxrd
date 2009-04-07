#ifndef QXRDINTEGRATORPLOT_H
#define QXRDINTEGRATORPLOT_H

#include <qwt_plot.h>

class QxrdIntegratorPlot : public QwtPlot
{
  Q_OBJECT;

public:
    QxrdIntegratorPlot(QWidget *parent=0);
};

#endif // QXRDINTEGRATORPLOT_H
