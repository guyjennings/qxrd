#include "qxrdplot.h"

#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include "qxrdplotmeasurer.h"
#include "qxrdplotzoomer.h"
#include <stdio.h>
#include <qwt_scale_engine.h>
#include <QMenu>
#include <QContextMenuEvent>

QxrdPlot::QxrdPlot(QWidget *parent)
  : QwtPlot(parent),
  m_XMouse(this,"xMouse",0),
  m_YMouse(this,"yMouse",0),
  m_XAxisLog(this,"xAxisLog",0),
  m_YAxisLog(this,"yAxisLog",0),
  m_X2AxisLog(this,"x2AxisLog",0),
  m_Y2AxisLog(this,"y2AxisLog",0),
  m_Legend(NULL),
  m_Zoomer(NULL),
  m_Panner(NULL),
  m_Magnifier(NULL),
  m_Measurer(NULL)
{
  for (int i=0; i<QwtPlot::axisCnt; i++) {
    m_IsLog[i] = 0;
  }

  setCanvasBackground(QColor(Qt::white));

  m_Zoomer = QxrdPlotZoomerPtr(new QxrdPlotZoomer(QwtPlotCanvasPtr(canvas()), QxrdPlotPtr(this)));
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);

  m_Legend = QwtLegendPtr(new QwtLegend(this));
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  m_Panner = QwtPlotPannerPtr(new QwtPlotPanner(canvas()));
  m_Panner -> setEnabled(true);
  m_Panner -> setMouseButton(Qt::MidButton);
  m_Panner -> setAxisEnabled(QwtPlot::yRight, false);

  m_Magnifier = QwtPlotMagnifierPtr(new QwtPlotMagnifier(canvas()));
  m_Magnifier -> setEnabled(true);
  m_Magnifier -> setMouseButton(Qt::NoButton);
  m_Magnifier -> setAxisEnabled(QwtPlot::yRight, false);

  m_Measurer = QxrdPlotMeasurerPtr(new QxrdPlotMeasurer(QwtPlotCanvasPtr(canvas()), QxrdPlotPtr(this)));
  m_Measurer -> setEnabled(false);

  setAxisLabelRotation(QwtPlot::yLeft, -90);
  setAxisLabelAlignment(QwtPlot::yLeft, Qt::AlignVCenter);

  autoScale();

  connect(prop_XAxisLog(), SIGNAL(changedValue(int)), this, SLOT(setXAxisLog(int)));
  connect(prop_YAxisLog(), SIGNAL(changedValue(int)), this, SLOT(setYAxisLog(int)));
  connect(prop_X2AxisLog(), SIGNAL(changedValue(int)), this, SLOT(setX2AxisLog(int)));
  connect(prop_Y2AxisLog(), SIGNAL(changedValue(int)), this, SLOT(setY2AxisLog(int)));
}

QxrdPlot::~QxrdPlot()
{
}

void QxrdPlot::readSettings(QxrdSettings &settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdPlot::writeSettings(QxrdSettings &settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdPlot::setPlotCurveStyle(int index, QwtPlotCurvePtr curve)
{
  const int nColors = 10;
  const int nSymbols = 4;
  const int nStyles = 3;

  int colorIndex = index % nColors;
  int symbolIndex = (index / nColors) % nSymbols;
  int styleIndex = (index / (nColors * nSymbols)) % nStyles;

  QPen pen;
  QwtSymbol symb;
  QBrush brush;

  switch (colorIndex) {
  case 0:
    pen = QPen(Qt::black);
    break;
  case 1:
    pen = QPen(Qt::red);
    break;
  case 2:
    pen = QPen(Qt::green);
    break;
  case 3:
    pen = QPen(Qt::blue);
    break;
  case 4:
    pen = QPen(Qt::cyan);
    break;
  case 5:
    pen = QPen(Qt::magenta);
    break;
  case 6:
    pen = QPen(Qt::darkRed);
    break;
  case 7:
    pen = QPen(Qt::darkGreen);
    break;
  case 8:
    pen = QPen(Qt::darkBlue);
    break;
  case 9:
    pen = QPen(Qt::darkYellow);
    break;
  }

  switch (styleIndex) {
  case 0:
    pen.setStyle(Qt::SolidLine);
    break;
  case 1:
    pen.setStyle(Qt::DashLine);
    break;
  case 2:
    pen.setStyle(Qt::DotLine);
    break;
  case 3:
    pen.setStyle(Qt::DashDotLine);
    break;
  }

  symb.setPen(pen);
  symb.setBrush(QBrush(pen.color()));
  symb.setSize(7,7);

  switch (symbolIndex) {
  case 0:
    symb.setStyle(QwtSymbol::Ellipse);
    break;
  case 1:
    symb.setStyle(QwtSymbol::Rect);
    break;
  case 2:
    symb.setStyle(QwtSymbol::XCross);
    break;
  case 3:
    symb.setStyle(QwtSymbol::Cross);
    break;
  }

  curve -> setPen(pen);
  curve -> setSymbol(symb);
}

void QxrdPlot::autoScale()
{
  setAxisAutoScale(QwtPlot::yLeft);
  setAxisAutoScale(QwtPlot::xBottom);

  m_Zoomer -> setZoomBase();

//
//  replot();
}

void QxrdPlot::zoomIn()
{
  m_Zoomer -> zoom(1);
}

void QxrdPlot::zoomOut()
{
  m_Zoomer -> zoom(-1);
}

void QxrdPlot::enableZooming()
{
  m_Zoomer       -> setEnabled(true);
  m_Measurer     -> setEnabled(false);
}

void QxrdPlot::enableMeasuring()
{
  m_Zoomer   -> setEnabled(false);
  m_Measurer -> setEnabled(true);
}

void QxrdPlot::onLegendClicked(QwtPlotItem *item)
{
  emit printMessage(QDateTime::currentDateTime(),
                    tr("QxrdPlot::onLegendClicked(%1)").arg(item->title().text()));
}

void QxrdPlot::onLegendChecked(QwtPlotItem *item, bool checked)
{
  emit printMessage(QDateTime::currentDateTime(),
                    tr("QxrdPlot::onLegendChecked(%1,%2)").arg(item->title().text()).arg(checked));
}

void QxrdPlot::setXAxisLog(int isLog)
{
  setLogAxis(QwtPlot::xBottom, isLog);
}

void QxrdPlot::setYAxisLog(int isLog)
{
  setLogAxis(QwtPlot::yLeft, isLog);
}

void QxrdPlot::setX2AxisLog(int isLog)
{
  setLogAxis(QwtPlot::xTop, isLog);
}

void QxrdPlot::setY2AxisLog(int isLog)
{
  setLogAxis(QwtPlot::yRight, isLog);
}

void QxrdPlot::setLogAxis(int axis, int isLog)
{
  if (axis >= 0 && axis < QwtPlot::axisCnt) {
    m_IsLog[axis] = isLog;

    if (isLog) {
      setAxisScaleEngine(axis, new QwtLog10ScaleEngine);
    } else {
      setAxisScaleEngine(axis, new QwtLinearScaleEngine);
    }

    replot();
  }
}

void QxrdPlot::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu plotMenu(NULL, NULL);

  QAction *xLog = plotMenu.addAction("Log X Axis");
  QAction *yLog = plotMenu.addAction("Log Y Axis");
  QAction *auSc = plotMenu.addAction("Autoscale");

  xLog->setCheckable(true);
  yLog->setCheckable(true);
  xLog->setChecked(get_XAxisLog());
  yLog->setChecked(get_YAxisLog());

  QAction *action = plotMenu.exec(event->globalPos());

  if (action == xLog) {
    set_XAxisLog(!get_XAxisLog());
  } else if (action == yLog) {
    set_YAxisLog(!get_YAxisLog());
  } else if (action == auSc) {
    autoScale();
  }

  event->accept();
}

int QxrdPlot::logAxis(int axis)
{
  return m_IsLog[axis];
//  QwtScaleEngine *se = axisScaleEngine(axis);

//  if (se) {
//    QSharedPointer<QwtScaleTransformation> tr(se->transformation());

//    if (tr) {
//      return tr->type() == QwtScaleTransformation::Log10;
//    }
//  }

//  return false;
}

QwtText QxrdPlot::trackerText(const QwtDoublePoint &pos)
{
  set_XMouse(pos.x());
  set_YMouse(pos.y());

  return tr("%1, %2").arg(pos.x()).arg(pos.y());
}
