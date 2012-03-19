#ifndef QXRDPLOT_H
#define QXRDPLOT_H

#include "qcepproperty.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include "qxrdplotmeasurer-ptr.h"

class QxrdPlot : public QwtPlot
{
  Q_OBJECT

public:
  QxrdPlot(QWidget *parent = 0);
  typedef QwtPlot inherited;
  virtual ~QxrdPlot();

public:
//  virtual void setSaver(QxrdSettingsSaverPtr saver);

  void setPlotCurveStyle(int index, QwtPlotCurve *curve);
  virtual QwtText trackerText(const QwtDoublePoint &pos);

  void contextMenuEvent(QContextMenuEvent *event);
  void updateZoomer();

public slots:
  void autoScale();
  void zoomIn();
  void zoomOut();

  void enableZooming();
  void enableMeasuring();

  void onLegendClicked(QwtPlotItem *item);
  void onLegendChecked(QwtPlotItem *item, bool checked);

  void setXAxisLog(int isLog);
  void setYAxisLog(int isLog);
  void setX2AxisLog(int isLog);
  void setY2AxisLog(int isLog);

  void setLogAxis(int axis, int isLog);
  int logAxis(int axis);

//  void setCustomTracker(QwtPlotPicker *tracker);
//  void setCustomZoomer(QwtPlotZoomer *zoomer);

protected:
  QwtLegend        *m_Legend;
  QwtPlotZoomer    *m_Zoomer;
  QwtPlotPanner    *m_Panner;
  QwtPlotMagnifier *m_Magnifier;
  QxrdPlotMeasurerPtr m_Measurer;

  int                 m_IsLog[QwtPlot::axisCnt];
};

#endif // QXRDPLOT_H
