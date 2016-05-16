#ifndef QXRDSYNCHRONIZEDACQUISITIONPLOT_H
#define QXRDSYNCHRONIZEDACQUISITIONPLOT_H

#include "qcepplot.h"
#include "qxrdsynchronizedacquisitionplotsettings-ptr.h"

class QxrdSynchronizedAcquisitionPlot : public QcepPlot
{
    Q_OBJECT
public:
    explicit QxrdSynchronizedAcquisitionPlot(QWidget *parent = 0);
  void init(QxrdSynchronizedAcquisitionPlotSettingsWPtr settings);

signals:

public slots:

};

#endif // QXRDSYNCHRONIZEDACQUISITIONPLOT_H
