#ifndef QXRDCENTERFINDERPLOT_H
#define QXRDCENTERFINDERPLOT_H

#include <qwt_plot.h>

class QxrdCenterFinderPlot : public QwtPlot
{
  Q_OBJECT;

public:
    QxrdCenterFinderPlot(QWidget *parent=0);
};

#endif // QXRDCENTERFINDERPLOT_H
