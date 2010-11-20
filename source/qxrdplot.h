#ifndef QXRDPLOT_H
#define QXRDPLOT_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qxrdforwardtypes.h"
#include "qxrdsettings.h"
#include <qwt_plot.h>

class QxrdPlot : public QwtPlot
{
  Q_OBJECT;

  Q_PROPERTY(double xMouse READ get_XMouse WRITE set_XMouse STORED false);
  QCEP_DOUBLE_PROPERTY(XMouse);

  Q_PROPERTY(double yMouse READ get_YMouse WRITE set_YMouse STORED false);
  QCEP_DOUBLE_PROPERTY(YMouse);

  Q_PROPERTY(int xAxisLog READ get_XAxisLog WRITE set_XAxisLog);
  QCEP_INTEGER_PROPERTY(XAxisLog);

  Q_PROPERTY(int yAxisLog READ get_YAxisLog WRITE set_YAxisLog);
  QCEP_INTEGER_PROPERTY(YAxisLog);

  Q_PROPERTY(int x2AxisLog READ get_X2AxisLog WRITE set_X2AxisLog);
  QCEP_INTEGER_PROPERTY(X2AxisLog);

  Q_PROPERTY(int y2AxisLog READ get_Y2AxisLog WRITE set_Y2AxisLog);
  QCEP_INTEGER_PROPERTY(Y2AxisLog);

public:
  QxrdPlot(QWidget *parent = 0);
  typedef QwtPlot inherited;
  virtual ~QxrdPlot();

public:
  virtual void readSettings(QxrdSettings &settings, QString section);
  virtual void writeSettings(QxrdSettings &settings, QString section);

  void setPlotCurveStyle(int index, QwtPlotCurvePtr curve);
  virtual QwtText trackerText(const QwtDoublePoint &pos);

  void contextMenuEvent(QContextMenuEvent *event);

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

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
  QwtLegendPtr        m_Legend;
  QwtPlotZoomerPtr    m_Zoomer;
  QwtPlotPannerPtr    m_Panner;
  QwtPlotMagnifierPtr m_Magnifier;
  QxrdPlotMeasurerPtr m_Measurer;

  int                 m_IsLog[QwtPlot::axisCnt];
};

#endif // QXRDPLOT_H
