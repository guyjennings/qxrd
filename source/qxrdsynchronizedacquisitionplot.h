#ifndef QXRDSYNCHRONIZEDACQUISITIONPLOT_H
#define QXRDSYNCHRONIZEDACQUISITIONPLOT_H

#include "qxrdplot.h"
#include "qxrdsynchronizedacquisitionplotsettings.h"

class QxrdSynchronizedAcquisitionPlot : public QxrdPlot
{
    Q_OBJECT
public:
    explicit QxrdSynchronizedAcquisitionPlot(QWidget *parent = 0);
  void init(QxrdSynchronizedAcquisitionPlotSettingsWPtr settings);

signals:

public slots:

};

#endif // QXRDSYNCHRONIZEDACQUISITIONPLOT_H
