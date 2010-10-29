#ifndef QXRDPLOT_H
#define QXRDPLOT_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qxrdforwardtypes.h"
#include <qwt_plot.h>

class QxrdPlot : public QwtPlot
{
  Q_OBJECT;

  Q_PROPERTY(double xMouse READ get_XMouse WRITE set_XMouse STORED false);
  QCEP_DOUBLE_PROPERTY(XMouse);

  Q_PROPERTY(double yMouse READ get_YMouse WRITE set_YMouse STORED false);
  QCEP_DOUBLE_PROPERTY(YMouse);

public:
  QxrdPlot(QWidget *parent = 0);
  typedef QwtPlot inherited;
  virtual ~QxrdPlot();

public:
  void setPlotCurveStyle(int index, QwtPlotCurvePtr curve);
  virtual QwtText trackerText(const QwtDoublePoint &pos);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  void autoScale();
  void zoomIn();
  void zoomOut();

  void enableZooming();
  void enableMeasuring();

  void onLegendClicked(QwtPlotItem *item);
  void onLegendChecked(QwtPlotItem *item, bool checked);

  void setLogAxis(int axis, int isLog);

//  void setCustomTracker(QwtPlotPicker *tracker);
//  void setCustomZoomer(QwtPlotZoomer *zoomer);

protected:
  QwtLegendPtr        m_Legend;
  QwtPlotZoomerPtr    m_Zoomer;
  QwtPlotPannerPtr    m_Panner;
  QwtPlotMagnifierPtr m_Magnifier;
  QxrdPlotMeasurerPtr m_Measurer;
};

#endif // QXRDPLOT_H
