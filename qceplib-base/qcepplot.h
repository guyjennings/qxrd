#ifndef QCEPPLOT_H
#define QCEPPLOT_H

#include "qceplib_global.h"
#include <QObject>
#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include "qcepplotsettings-ptr.h"
#include "qcepplotmeasurer-ptr.h"

class QCEP_EXPORT QcepPlot : public QwtPlot
{
  Q_OBJECT

public:
  QcepPlot(QWidget *parent = 0);
  virtual ~QcepPlot();
  //TODO: change to QObjectWPtr...
  void initialize(QcepPlotSettingsWPtr settings);

public:
  void setPlotCurveStyle(int index, QwtPlotCurve *curve);
  virtual QwtText trackerTextF(const QPointF &pos);

  void contextMenuEvent(QContextMenuEvent *event);
  void updateZoomer();

public slots:
  virtual void autoScale();
  void printGraph();
  void zoomIn();
  void zoomOut();

  void enableZooming();
  void enableMeasuring();

  virtual void onLegendClicked(const QVariant &itemInfo, int index);
  virtual void onLegendChecked(const QVariant &itemInfo, bool on, int index);

  void setXAxisLog(bool isLog);
  void setYAxisLog(bool isLog);
  void setX2AxisLog(bool isLog);
  void setY2AxisLog(bool isLog);

  void setLogAxis(int axis, int isLog);
  int logAxis(int axis);

//  void setCustomTracker(QwtPlotPicker *tracker);
//  void setCustomZoomer(QwtPlotZoomer *zoomer);

protected:
  QcepPlotSettingsWPtr m_PlotSettings;
  QwtLegend           *m_Legend;
//  QwtPlotZoomer       *m_Zoomer;
//  QwtPlotPanner       *m_Panner;
//  QwtPlotMagnifier    *m_Magnifier;
//  QcepPlotMeasurerPtr  m_Measurer;

  int                  m_IsLog[QwtPlot::axisCnt];
};

#endif // QCEPPLOT_H
