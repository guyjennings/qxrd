#include "qcepplot.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QMenu>
#include <QContextMenuEvent>

#include <qwt_symbol.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_renderer.h>
#include <qwt_scale_engine.h>

#include "qcepplotzoomer.h"
#include "qcepplotmeasurer.h"
#include "qcepplotsettings.h"
#include "qcepapplication.h"

QcepPlot::QcepPlot(QWidget *parent)
  : QwtPlot(parent),
    m_PlotSettings(),
    m_Legend(NULL)
//    m_Zoomer(NULL),
//    m_Panner(NULL),
//    m_Magnifier(NULL),
//    m_Measurer(NULL)
{
  for (int i=0; i<QwtPlot::axisCnt; i++) {
    m_IsLog[i] = 0;
  }

  setCanvasBackground(QColor(Qt::green));
}

QcepPlot::~QcepPlot()
{
}

void QcepPlot::init(QcepPlotSettingsWPtr settings)
{
  m_PlotSettings = settings;

  setCanvasBackground(QColor(Qt::white));

//  m_Zoomer = new QcepPlotZoomer(canvas(), this);
//  m_Zoomer -> setStateMachine(new QwtPickerDragRectMachine());
//  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
//  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

//  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
//                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
//  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
//                              Qt::LeftButton, Qt::ControlModifier);

//  m_Zoomer -> setEnabled(true);

  m_Legend = new QwtLegend(this);
  m_Legend -> setDefaultItemMode(QwtLegendData::Checkable);

//  m_Panner = new QwtPlotPanner(canvas());
//  m_Panner -> setEnabled(true);
//  m_Panner -> setMouseButton(Qt::MidButton);
//  m_Panner -> setAxisEnabled(QwtPlot::yRight, false);

//  m_Magnifier = new QwtPlotMagnifier(canvas());
//  m_Magnifier -> setEnabled(true);
//  m_Magnifier -> setMouseButton(Qt::NoButton);
//  m_Magnifier -> setAxisEnabled(QwtPlot::yRight, false);

//  m_Measurer = new QcepPlotMeasurer(canvas(), this);
//  m_Measurer -> setEnabled(false);

  setAxisLabelRotation(QwtPlot::yLeft, -90);
  setAxisLabelAlignment(QwtPlot::yLeft, Qt::AlignVCenter);

  autoScale();

  QcepPlotSettingsPtr set(m_PlotSettings);

  if (set) {
    connect(set->prop_XAxisLog(), &QcepBoolProperty::valueChanged, this, &QcepPlot::setXAxisLog);
    connect(set->prop_YAxisLog(), &QcepBoolProperty::valueChanged, this, &QcepPlot::setYAxisLog);
    connect(set->prop_X2AxisLog(), &QcepBoolProperty::valueChanged, this, &QcepPlot::setX2AxisLog);
    connect(set->prop_Y2AxisLog(), &QcepBoolProperty::valueChanged, this, &QcepPlot::setY2AxisLog);

    setXAxisLog(set->get_XAxisLog());
    setYAxisLog(set->get_YAxisLog());
    setX2AxisLog(set->get_X2AxisLog());
    setY2AxisLog(set->get_Y2AxisLog());

    if (m_Legend) {
      int legendPos = set->get_LegendPosition();

      if (legendPos >= 0) {
        insertLegend(m_Legend, (QwtPlot::LegendPosition) legendPos);
      }
    }
  }

  if (m_Legend) {
    connect(m_Legend, &QwtLegend::clicked,      this, &QcepPlot::onLegendClicked);
    connect(m_Legend, &QwtLegend::checked, this, &QcepPlot::onLegendChecked);
  }
}

void QcepPlot::setPlotCurveStyle(int index, QwtPlotCurve *curve)
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
    pen = QPen(QColor(255,0,0)); // red
    break;
  case 1:
    pen = QPen(QColor(255,170,0)); // Orange
    break;
  case 2:
    pen = QPen(QColor(255,232,137)); // Yellow
    break;
  case 3:
    pen = QPen(QColor(0,255,0)); // Green
    break;
  case 4:
    pen = QPen(QColor(0,170,0)); // Dk green
    break;
  case 5:
    pen = QPen(QColor(0,255,255)); // Cyan
    break;
  case 6:
    pen = QPen(QColor(0,170,255)); // Sea blue
    break;
  case 7:
    pen = QPen(QColor(0,0,255)); // Blue
    break;
  case 8:
    pen = QPen(QColor(145,0,255)); // Violet
    break;
  case 9:
    pen = QPen(QColor(255,0,255)); // Magenta
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
    symb->setStyle(QwtSymbol::Triangle);
    break;
  case 3:
    symb->setStyle(QwtSymbol::DTriangle);
    break;
  }

  curve -> setPen(pen);
  curve -> setSymbol(symb);
}

void QcepPlot::autoScale()
{
  setAxisAutoScale(QwtPlot::yLeft);
  setAxisAutoScale(QwtPlot::xBottom);

//  m_Zoomer -> setZoomBase();

//
//  replot();
}

void QcepPlot::printGraph()
{
  QPrinter printer( QPrinter::HighResolution );

  QString docName = this->title().text();
  if ( !docName.isEmpty() )
  {
    docName.replace ( QRegExp ( QString::fromLatin1 ( "\n" ) ), tr ( " -- " ) );
    printer.setDocName ( docName );
  }

//  printer.setCreator( "Bode example" );
  printer.setOrientation( QPrinter::Landscape );

  QPrintDialog dialog( &printer );

  if ( dialog.exec() ) {
    QwtPlotRenderer renderer;

    if ( printer.colorMode() == QPrinter::GrayScale ) {
      renderer.setDiscardFlag( QwtPlotRenderer::DiscardBackground );
      renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasBackground );
      renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasFrame );
//      renderer.setLayoutFlag( QwtPlotRenderer::FrameWithScales );
    }

    double gw = this->width();
    double gh = this->height();
    double pw = printer.width();
    double ph = printer.height();
    double scal = qMin( pw/gw, ph/gh);

    pw = gw*scal;
    ph = gh*scal;

    QRectF rect(0,0, pw,ph);

    QPainter p(&printer);

    renderer.render( this, &p, rect);
  }
}

//TODO: remove
void QcepPlot::zoomIn()
{
//  m_Zoomer -> zoom(1);
}

void QcepPlot::zoomOut()
{
//  m_Zoomer -> zoom(-1);
}

//TODO: remove
void QcepPlot::updateZoomer()
{
//  if (m_Zoomer && m_Zoomer -> zoomRectIndex() == 0) {
//    m_Zoomer -> setZoomBase();
//  }

  replot();
}

//TODO: remove
void QcepPlot::enableZooming()
{
//  m_Zoomer       -> setEnabled(true);
//  m_Measurer     -> setEnabled(false);
}

//TODO: remove
void QcepPlot::enableMeasuring()
{
//  m_Zoomer   -> setEnabled(false);
//  m_Measurer -> setEnabled(true);
}

//TODO: remove
void QcepPlot::onLegendClicked(const QVariant & /*itemInfo*/, int /*index*/)
{
//  if (g_Application) {
//    g_Application->printMessage(tr("QxrdPlot::onLegendClicked(%1,%2)").arg(itemInfo.toString()).arg(index));
//  }
}

//TODO: remove
void QcepPlot::onLegendChecked(const QVariant &itemInfo, bool on, int /*index*/)
{
//  if (g_Application) {
//    g_Application->printMessage(tr("QxrdPlot::onLegendChecked(%1,%2,%3)").arg(itemInfo.toString()).arg(on).arg(index));
//  }

  QwtPlotItem *item = infoToItem(itemInfo);

  if (item) {
    QwtPlotCurve *pc = dynamic_cast<QwtPlotCurve*>(item);

    if (pc) {
      QPen pen = pc->pen();
      const QwtSymbol *oldsym = pc->symbol();
      QwtSymbol *sym = NULL;

      if (oldsym) {
       sym = new QwtSymbol(oldsym->style(), oldsym->brush(), oldsym->pen(), oldsym->size());
      }

      if (on) {
        pen.setWidth(3);
        if (sym) {
          sym->setSize(9,9);
        }
      } else {
        pen.setWidth(1);
        if (sym) {
          sym->setSize(5,5);
        }
      }
      pc->setPen(pen);
      if (sym) {
        pc->setSymbol(sym);
      }
    }

    replot();
  }
}

void QcepPlot::setXAxisLog(bool isLog)
{
  setLogAxis(QwtPlot::xBottom, isLog);
}

void QcepPlot::setYAxisLog(bool isLog)
{
  setLogAxis(QwtPlot::yLeft, isLog);
}

void QcepPlot::setX2AxisLog(bool isLog)
{
  setLogAxis(QwtPlot::xTop, isLog);
}

void QcepPlot::setY2AxisLog(bool isLog)
{
  setLogAxis(QwtPlot::yRight, isLog);
}

void QcepPlot::setLogAxis(int axis, int isLog)
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

void QcepPlot::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu plotMenu(NULL, NULL);

  QAction *xLog = plotMenu.addAction("Log X Axis");
  QAction *yLog = plotMenu.addAction("Log Y Axis");
  QAction *auSc = plotMenu.addAction("Autoscale");
  QAction *prGr = plotMenu.addAction("Print Graph...");

  QcepPlotSettingsPtr set(m_PlotSettings);

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
    } else if (action == prGr) {
      printGraph();
    }
  }

  event->accept();
}

int QcepPlot::logAxis(int axis)
{
  return m_IsLog[axis];
}

QwtText QcepPlot::trackerTextF(const QPointF &pos)
{
  QcepPlotSettingsPtr set(m_PlotSettings);

  if (set) {
    set->set_XMouse(pos.x());
    set->set_YMouse(pos.y());
  }

  return tr("%1, %2").arg(pos.x()).arg(pos.y());
}
