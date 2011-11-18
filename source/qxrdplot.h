#ifndef QXRDPLOT_H
#define QXRDPLOT_H

#include "qcepproperty.h"
#include "qxrdsettings.h"
#include <qwt_plot.h>

class QwtPlotCurve;
class QwtPlotZoomer;
class QwtPlotPanner;
class QwtPlotMagnifier;
class QxrdPlotMeasurer;

class QxrdPlot : public QwtPlot
{
  Q_OBJECT

  Q_PROPERTY(double xMouse READ get_XMouse WRITE set_XMouse STORED false)
  QCEP_DOUBLE_PROPERTY(XMouse)

  Q_PROPERTY(double yMouse READ get_YMouse WRITE set_YMouse STORED false)
  QCEP_DOUBLE_PROPERTY(YMouse)

  Q_PROPERTY(int xAxisLog READ get_XAxisLog WRITE set_XAxisLog)
  QCEP_INTEGER_PROPERTY(XAxisLog)

  Q_PROPERTY(int yAxisLog READ get_YAxisLog WRITE set_YAxisLog)
  QCEP_INTEGER_PROPERTY(YAxisLog)

  Q_PROPERTY(int x2AxisLog READ get_X2AxisLog WRITE set_X2AxisLog)
  QCEP_INTEGER_PROPERTY(X2AxisLog)

  Q_PROPERTY(int y2AxisLog READ get_Y2AxisLog WRITE set_Y2AxisLog)
  QCEP_INTEGER_PROPERTY(Y2AxisLog)

public:
  QxrdPlot(QWidget *parent = 0);
  typedef QwtPlot inherited;
  virtual ~QxrdPlot();

public:
  virtual void readSettings(QSettings &settings, QString section);
  virtual void writeSettings(QSettings &settings, QString section);

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
  QxrdPlotMeasurer *m_Measurer;

  int                 m_IsLog[QwtPlot::axisCnt];
};

#endif // QXRDPLOT_H
