#include "qxrdplot.h"

#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_picker_machine.h>
#include "qxrdplotmeasurer.h"
#include "qxrdplotzoomer.h"
#include <stdio.h>
#include <qwt_scale_engine.h>
#include <QMenu>
#include <QContextMenuEvent>
#include "qxrdapplication.h"

QxrdPlot::QxrdPlot(QWidget *parent)
  : QwtPlot(parent),
    m_PlotSettings(),
    m_Legend(NULL),
    m_Zoomer(NULL),
    m_Panner(NULL),
    m_Magnifier(NULL),
    m_Measurer(NULL)
{
  for (int i=0; i<QwtPlot::axisCnt; i++) {
    m_IsLog[i] = 0;
  }
}

QxrdPlot::~QxrdPlot()
{
}

void QxrdPlot::init(QxrdPlotSettingsWPtr settings)
{
  m_PlotSettings = settings;

  setCanvasBackground(QColor(Qt::white));

  m_Zoomer = new QxrdPlotZoomer(canvas(), this);
  m_Zoomer -> setStateMachine(new QwtPickerDragRectMachine());
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);

  m_Legend = new QwtLegend(this);
  m_Legend -> setDefaultItemMode(QwtLegendData::Checkable);

  m_Panner = new QwtPlotPanner(canvas());
  m_Panner -> setEnabled(true);
  m_Panner -> setMouseButton(Qt::MidButton);
  m_Panner -> setAxisEnabled(QwtPlot::yRight, false);

  m_Magnifier = new QwtPlotMagnifier(canvas());
  m_Magnifier -> setEnabled(true);
  m_Magnifier -> setMouseButton(Qt::NoButton);
  m_Magnifier -> setAxisEnabled(QwtPlot::yRight, false);

  m_Measurer = new QxrdPlotMeasurer(canvas(), this);
  m_Measurer -> setEnabled(false);

  setAxisLabelRotation(QwtPlot::yLeft, -90);
  setAxisLabelAlignment(QwtPlot::yLeft, Qt::AlignVCenter);

  autoScale();

  QxrdPlotSettingsPtr set(m_PlotSettings);

  if (set) {
    connect(set->prop_XAxisLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(setXAxisLog(bool)));
    connect(set->prop_YAxisLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(setYAxisLog(bool)));
    connect(set->prop_X2AxisLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(setX2AxisLog(bool)));
    connect(set->prop_Y2AxisLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(setY2AxisLog(bool)));

    setXAxisLog(set->get_XAxisLog());
    setYAxisLog(set->get_YAxisLog());
    setX2AxisLog(set->get_X2AxisLog());
    setY2AxisLog(set->get_Y2AxisLog());
  }
}

void QxrdPlot::setPlotCurveStyle(int index, QwtPlotCurve *curve)
{
  const int nColors = 10;
  const int nSymbols = 4;
  const int nStyles = 3;

  int colorIndex = index % nColors;
  int symbolIndex = (index / nColors) % nSymbols;
  int styleIndex = (index / (nColors * nSymbols)) % nStyles;

  QPen pen;
  QwtSymbol *symb = new QwtSymbol();
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

  symb->setPen(pen);
  symb->setBrush(QBrush(pen.color()));
  symb->setSize(5,5);

  switch (symbolIndex) {
  case 0:
    symb->setStyle(QwtSymbol::Ellipse);
    break;
  case 1:
    symb->setStyle(QwtSymbol::Rect);
    break;
  case 2:
    symb->setStyle(QwtSymbol::XCross);
    break;
  case 3:
    symb->setStyle(QwtSymbol::Cross);
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

void QxrdPlot::updateZoomer()
{
  if (m_Zoomer && m_Zoomer -> zoomRectIndex() == 0) {
    m_Zoomer -> setZoomBase();
  }

  replot();
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

void QxrdPlot::onLegendClicked(const QVariant &itemInfo, int index)
{
  if (g_Application) {
    g_Application->printMessage(tr("QxrdPlot::onLegendClicked(%1,%2)").arg(itemInfo.toString()).arg(index));
  }
}

void QxrdPlot::onLegendChecked(const QVariant &itemInfo, bool on, int index)
{
  if (g_Application) {
    g_Application->printMessage(tr("QxrdPlot::onLegendChecked(%1,%2,%3)").arg(itemInfo.toString()).arg(on).arg(index));
  }

  QwtPlotItem *item = infoToItem(itemInfo);

  if (item) {
    QwtPlotCurve *pc = dynamic_cast<QwtPlotCurve*>(item);

    if (pc) {
      QPen pen = pc->pen();
      const QwtSymbol *oldsym = pc->symbol();
      QwtSymbol *sym = new QwtSymbol(oldsym->style(), oldsym->brush(), oldsym->pen(), oldsym->size());

      if (on) {
        pen.setWidth(3);
        sym->setSize(9,9);
      } else {
        pen.setWidth(1);
        sym->setSize(5,5);
      }
      pc->setPen(pen);
      pc->setSymbol(sym);
    }

    replot();
  }
}

void QxrdPlot::setXAxisLog(bool isLog)
{
  setLogAxis(QwtPlot::xBottom, isLog);
}

void QxrdPlot::setYAxisLog(bool isLog)
{
  setLogAxis(QwtPlot::yLeft, isLog);
}

void QxrdPlot::setX2AxisLog(bool isLog)
{
  setLogAxis(QwtPlot::xTop, isLog);
}

void QxrdPlot::setY2AxisLog(bool isLog)
{
  setLogAxis(QwtPlot::yRight, isLog);
}

void QxrdPlot::setLogAxis(int axis, int isLog)
{
  if (axis >= 0 && axis < QwtPlot::axisCnt) {
    m_IsLog[axis] = isLog;

    if (isLog) {
      setAxisScaleEngine(axis, new QwtLogScaleEngine);
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

  QxrdPlotSettingsPtr set(m_PlotSettings);

  if (set) {
    xLog->setCheckable(true);
    yLog->setCheckable(true);
    xLog->setChecked(set->get_XAxisLog());
    yLog->setChecked(set->get_YAxisLog());

    QAction *action = plotMenu.exec(event->globalPos());

    if (action == xLog) {
      set->toggle_XAxisLog();
    } else if (action == yLog) {
      set->toggle_YAxisLog();
    } else if (action == auSc) {
      autoScale();
    }
  }

  event->accept();
}

int QxrdPlot::logAxis(int axis)
{
  return m_IsLog[axis];
}

QwtText QxrdPlot::trackerTextF(const QPointF &pos)
{
  QxrdPlotSettingsPtr set(m_PlotSettings);

  if (set) {
    set->set_XMouse(pos.x());
    set->set_YMouse(pos.y());
  }

  return tr("%1, %2").arg(pos.x()).arg(pos.y());
}
