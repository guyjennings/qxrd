#ifndef QXRDSYNCHRONIZEDACQUISITIONPLOT_H
#define QXRDSYNCHRONIZEDACQUISITIONPLOT_H

#include "qxrdlib_global.h"
#include "qcepplot.h"
#include "qxrdsynchronizedacquisitionplotsettings-ptr.h"

class QXRD_EXPORT QxrdSynchronizedAcquisitionPlot : public QcepPlot
{
    Q_OBJECT
public:
    explicit QxrdSynchronizedAcquisitionPlot(QWidget *parent = 0);
  void init(QxrdSynchronizedAcquisitionPlotSettingsWPtr settings);

signals:

public slots:

};

#endif // QXRDSYNCHRONIZEDACQUISITIONPLOT_H
