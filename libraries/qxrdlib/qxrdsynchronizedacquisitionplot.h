#ifndef QXRDSYNCHRONIZEDACQUISITIONPLOT_H
#define QXRDSYNCHRONIZEDACQUISITIONPLOT_H

#include "qxrdlib_global.h"
#include "qcepplot.h"
#include "qxrdsynchronizedacquisitionplotsettings-ptr.h"

//TODO: is class used?
class QXRD_EXPORT QxrdSynchronizedAcquisitionPlot : public QcepPlot
{
  Q_OBJECT
public:
  explicit QxrdSynchronizedAcquisitionPlot(QWidget *parent = 0);
  //TODO: change to QcepObjectWPtr...
  void initialize(QxrdSynchronizedAcquisitionPlotSettingsWPtr settings);

signals:

public slots:

};

#endif // QXRDSYNCHRONIZEDACQUISITIONPLOT_H
