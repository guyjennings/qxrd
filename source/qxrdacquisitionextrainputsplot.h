#ifndef QXRDACQUISITIONEXTRAINPUTSPLOT_H
#define QXRDACQUISITIONEXTRAINPUTSPLOT_H

#include "qcepmacros.h"

#include "qxrdplot.h"
#include "qwt_plot_curve.h"

class QxrdAcquisitionExtraInputsPlot : public QxrdPlot
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionExtraInputsPlot(QWidget *parent = 0);
  
signals:
  
public slots:
  void setNChannels(int nch);
  void plotChannel(int ch, int i0, int i1, QcepDoubleVector x, QcepDoubleVector y);

private:
  QVector<QwtPlotCurve*> m_RawData;
  QVector<QwtPlotCurve*> m_Selections;
  QVector<QColor>        m_Colors;
};

#endif // QXRDACQUISITIONEXTRAINPUTSPLOT_H
