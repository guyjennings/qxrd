#ifndef QXRDPLOT_H
#define QXRDPLOT_H

#include "qcepproperty.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include "qxrdplotmeasurer-ptr.h"
#include "qxrdplotsettings.h"

class QxrdPlot : public QwtPlot
{
  Q_OBJECT

public:
  QxrdPlot(QWidget *parent = 0);
  typedef QwtPlot inherited;
  virtual ~QxrdPlot();
  void init(QxrdPlotSettingsWPtr settings);

public:
//  virtual void setSaver(QcepSettingsSaverPtr saver);

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

  void onLegendClicked(const QVariant &itemInfo, int index);
  void onLegendChecked(const QVariant &itemInfo, bool on, int index);

  void setXAxisLog(bool isLog);
  void setYAxisLog(bool isLog);
  void setX2AxisLog(bool isLog);
  void setY2AxisLog(bool isLog);

  void setLogAxis(int axis, int isLog);
  int logAxis(int axis);

//  void setCustomTracker(QwtPlotPicker *tracker);
//  void setCustomZoomer(QwtPlotZoomer *zoomer);

protected:
  QxrdPlotSettingsWPtr m_PlotSettings;
  QwtLegend           *m_Legend;
  QwtPlotZoomer       *m_Zoomer;
  QwtPlotPanner       *m_Panner;
  QwtPlotMagnifier    *m_Magnifier;
  QxrdPlotMeasurerPtr  m_Measurer;

  int                  m_IsLog[QwtPlot::axisCnt];
};

#endif // QXRDPLOT_H
