#include "qxrdplotwidget.h"
#include "qxrdplotwidgetsettings.h"
#include "qxrdplotcommand.h"
#include <QLayout>
#include <QToolButton>
#include <QSpacerItem>
#include <QMenu>
#include <QPrinter>
#include <QPrintDialog>
#include "qxrdautoscalecommand.h"
#include "qxrdprintplotcommand.h"
#include "qxrdplotpreferencescommand.h"
#include "qxrdzoomincommand.h"
#include "qxrdzoomoutcommand.h"
#include "qxrdzoomallcommand.h"
#include "qxrddisplaysubmenucommand.h"
#include "qcepplotzoomer.h"
#include <qwt_symbol.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_renderer.h>

QxrdPlotWidget::QxrdPlotWidget(QWidget *parent) :
  QWidget(parent)
{
  for (int i=0; i<QwtPlot::axisCnt; i++) {
    m_IsLog[i] = 0;
  }

  setupUi(this);

//  QLayout     *oldLayout = layout();

  QGridLayout *layout = new QGridLayout(m_PlotTools);

  layout -> setContentsMargins(0,0,0,0);
  layout -> setSpacing(2);

  m_PlotTools->setLayout(layout);

  m_Plot->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(m_Plot, &QWidget::customContextMenuRequested,
          this,   &QxrdPlotWidget::contextMenu);

  connect(&m_Timer, &QTimer::timeout, this, &QxrdPlotWidget::updateDisplayedImage);

  m_Timer.start(1000);
}

QxrdPlotWidget::~QxrdPlotWidget()
{
}

void QxrdPlotWidget::initialize(QxrdPlotWidgetSettingsWPtr settings)
{
  m_Settings = settings;

  m_Plot -> setCanvasBackground(QBrush(Qt::cyan));

  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomInCommand("Zoom In", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomOutCommand("Zoom Out", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdZoomAllCommand("Zoom All", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdPlotPreferencesCommand("Plot Preferences...", this, settings)));

  addPlotCommand(QxrdPlotCommandPtr(new QxrdAutoScaleCommand("Auto Scale", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdPrintPlotCommand("Print Graph...", this, settings)));
  addPlotCommand(QxrdPlotCommandPtr(new QxrdDisplaySubmenuCommand("Display", this, settings)));

  QWidget* canvas = m_Plot -> canvas();

  m_Zoomer = new QcepPlotZoomer(canvas, m_Plot);
  m_Zoomer -> setStateMachine(new QwtPickerDragRectMachine());
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);

  m_Legend = new QwtLegend(m_Plot);
  m_Legend -> setDefaultItemMode(QwtLegendData::Checkable);

  m_Panner = new QwtPlotPanner(canvas);
  m_Panner -> setEnabled(true);
  m_Panner -> setMouseButton(Qt::MidButton);
  m_Panner -> setAxisEnabled(QwtPlot::yRight, false);

  m_Magnifier = new QwtPlotMagnifier(canvas);
  m_Magnifier -> setEnabled(true);
  m_Magnifier -> setMouseButton(Qt::NoButton);
  m_Magnifier -> setAxisEnabled(QwtPlot::yRight, false);

  m_Measurer = new QcepPlotMeasurer(canvas, m_Plot);
  m_Measurer -> setEnabled(false);

  m_Plot -> setAxisLabelRotation(QwtPlot::yLeft, -90);
  m_Plot -> setAxisLabelAlignment(QwtPlot::yLeft, Qt::AlignVCenter);

  QxrdPlotWidgetSettingsPtr set(m_Settings);

  if (set) {
    connect(set->prop_XAxisLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(setXAxisLog(bool)));
    connect(set->prop_YAxisLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(setYAxisLog(bool)));
    connect(set->prop_X2AxisLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(setX2AxisLog(bool)));
    connect(set->prop_Y2AxisLog(), SIGNAL(valueChanged(bool,int)), this, SLOT(setY2AxisLog(bool)));
    connect(set->prop_LegendPosition(), SIGNAL(valueChanged(int,int)), this, SLOT(setLegendPosition(int)));

    setXAxisLog(set->get_XAxisLog());
    setYAxisLog(set->get_YAxisLog());
    setX2AxisLog(set->get_X2AxisLog());
    setY2AxisLog(set->get_Y2AxisLog());
    setLegendPosition(set->get_LegendPosition());

    connect(m_Legend, SIGNAL(clicked(const QVariant &,int)),      this, SLOT(onLegendClicked(const QVariant&, int)));
    connect(m_Legend, SIGNAL(checked(const QVariant &,bool,int)), this, SLOT(onLegendChecked(const QVariant&, bool, int)));
  }
}

void QxrdPlotWidget::addPlotCommand(QxrdPlotCommandPtr cmd)
{
  m_PlotCommands.append(cmd);

  if (cmd) {
    QToolButton *button = cmd->toolButton();

    if (button) {
      QLayout *layout = m_PlotTools->layout();

      if (layout) {
        layout->addWidget(button);
      }
    }
  }
}

void QxrdPlotWidget::addPlotCommandSpacer()
{
  QSpacerItem *spacer =
      new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

  QLayout *layout = m_PlotTools->layout();

  if (layout) {
    layout->addItem(spacer);
  }
}

void QxrdPlotWidget::contextMenu(const QPoint &pos)
{
  QMenu plotMenu(NULL, NULL);

  foreach (QxrdPlotCommandPtr cmd, m_PlotCommands) {
    if (cmd) {
      QAction* act = cmd->contextMenuAction(pos);

      if (act) {
        plotMenu.addAction(act);
      }
    }
  }

  plotMenu.exec(mapToGlobal(pos));
}

void QxrdPlotWidget::disableCommands()
{
  foreach (QxrdPlotCommandPtr cmd, m_PlotCommands) {
    if (cmd) {
      cmd->disable();
    }
  }
}

void QxrdPlotWidget::onProcessedImageAvailable(QcepDoubleImageDataPtr img)
{
  m_NewImageData = img;
}

void QxrdPlotWidget::updateDisplayedImage()
{
  if (m_NewImageData) {
    m_ImageData = m_NewImageData;
    m_NewImageData = QcepDoubleImageDataPtr();

    m_Plot->onProcessedImageAvailable(m_ImageData, QcepMaskDataPtr());
  }
}

void QxrdPlotWidget::zoomIn()
{
  m_Zoomer -> zoom(1);
}

void QxrdPlotWidget::zoomAll()
{
  m_Plot -> setAxisAutoScale(QwtPlot::yLeft);
  m_Plot -> setAxisAutoScale(QwtPlot::xBottom);

  m_Zoomer -> setZoomBase();
}

void QxrdPlotWidget::zoomOut()
{
  m_Zoomer -> zoom(-1);
}

void QxrdPlotWidget::printGraph()
{
  QPrinter printer( QPrinter::HighResolution );

  QString docName = m_Plot->title().text();
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

    double gw = m_Plot->width();
    double gh = m_Plot->height();
    double pw = printer.width();
    double ph = printer.height();
    double scal = qMin( pw/gw, ph/gh);

    pw = gw*scal;
    ph = gh*scal;

    QRectF rect(0,0, pw,ph);

    QPainter p(&printer);

    renderer.render( m_Plot, &p, rect);
  }
}

void QxrdPlotWidget::setLegendPosition(int legendPosition)
{
  if (legendPosition >= 0) {
    m_Plot -> insertLegend(m_Legend, (QwtPlot::LegendPosition) legendPosition);
  }
}


void QxrdPlotWidget::onLegendClicked(const QVariant & /*itemInfo*/, int /*index*/)
{
//  if (g_Application) {
//    g_Application->printMessage(tr("QxrdPlot::onLegendClicked(%1,%2)").arg(itemInfo.toString()).arg(index));
//  }
}

void QxrdPlotWidget::onLegendChecked(const QVariant &itemInfo, bool on, int /*index*/)
{
//  if (g_Application) {
//    g_Application->printMessage(tr("QxrdPlot::onLegendChecked(%1,%2,%3)").arg(itemInfo.toString()).arg(on).arg(index));
//  }

  QwtPlotItem *item = m_Plot -> infoToItem(itemInfo);

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

    m_Plot -> replot();
  }
}

void QxrdPlotWidget::setXAxisLog(bool isLog)
{
  setLogAxis(QwtPlot::xBottom, isLog);
}

void QxrdPlotWidget::setYAxisLog(bool isLog)
{
  setLogAxis(QwtPlot::yLeft, isLog);
}

void QxrdPlotWidget::setX2AxisLog(bool isLog)
{
  setLogAxis(QwtPlot::xTop, isLog);
}

void QxrdPlotWidget::setY2AxisLog(bool isLog)
{
  setLogAxis(QwtPlot::yRight, isLog);
}

void QxrdPlotWidget::setLogAxis(int axis, int isLog)
{
  if (axis >= 0 && axis < QwtPlot::axisCnt) {
    m_IsLog[axis] = isLog;

    if (isLog) {
      m_Plot -> setAxisScaleEngine(axis, new QwtLogScaleEngine);
    } else {
      m_Plot -> setAxisScaleEngine(axis, new QwtLinearScaleEngine);
    }

    m_Plot -> replot();
  }
}
