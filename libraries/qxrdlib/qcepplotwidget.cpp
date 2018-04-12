#include "qcepplotwidget.h"
#include "qcepplotwidgetsettings.h"
#include "qcepplotcommand.h"
#include "qcepplotoverlay.h"
#include <QLayout>
#include <QToolButton>
#include <QSpacerItem>
#include <QMenu>
#include <QPrinter>
#include <QPrintDialog>
#include "qcepautoscalecommand.h"
#include "qcepprintplotcommand.h"
#include "qcepprintplotbutton.h"
#include "qcepplotpreferencescommand.h"
#include "qcepplotpreferencesbutton.h"
#include "qcepzoomincommand.h"
#include "qcepzoomoutcommand.h"
#include "qcepzoomallcommand.h"
#include "qcepaxissubmenucommand.h"
#include "qcepplotzoomer.h"
#include <qwt_symbol.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_renderer.h>
#include "qcepplotwidgetdialog.h"
#include "qcepcolormaplibrary.h"

QcepPlotWidget::QcepPlotWidget(QWidget *parent) :
  QWidget(parent),
  m_Legend(NULL),
  m_Zoomer(NULL),
  m_Panner(NULL),
  m_Magnifier(NULL),
  m_Measurer(NULL)
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
          this,   &QcepPlotWidget::contextMenu);
}

QcepPlotWidget::~QcepPlotWidget()
{
}

void QcepPlotWidget::initialize(QcepPlotWidgetSettingsWPtr settings)
{
  m_Settings = settings;

  //  m_Plot -> setCanvasBackground(QBrush(Qt::cyan));
  m_Plot -> setCanvasBackground(QBrush(Qt::white));

  addPlotCommand(QcepPlotCommandPtr(new QcepZoomInCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepZoomOutCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepZoomAllCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepPrintPlotButton(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepPlotPreferencesButton(this, settings)));

  addPlotCommand(QcepPlotCommandPtr(new QcepAutoScaleCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepPrintPlotCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepPlotPreferencesCommand(this, settings)));
  addPlotCommand(QcepPlotCommandPtr(new QcepAxisSubmenuCommand(this, settings)));

  QWidget* canvas = m_Plot -> canvas();

  m_Zoomer = new QcepPlotZoomer(m_Plot);
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

  m_Measurer = new QcepPlotMeasurer(m_Plot);
  m_Measurer -> setEnabled(false);

  m_Plot -> setAxisLabelRotation(QwtPlot::yLeft, -90);
  m_Plot -> setAxisLabelAlignment(QwtPlot::yLeft, Qt::AlignVCenter);

  QcepPlotWidgetSettingsPtr set(m_Settings);

  if (set) {
    connect(set->prop_XAxisVis(),  &QcepBoolProperty::valueChanged, [=](bool vis) {setAxisVis(QwtPlot::xBottom, vis);});
    connect(set->prop_YAxisVis(),  &QcepBoolProperty::valueChanged, [=](bool vis) {setAxisVis(QwtPlot::yLeft,   vis);});
    connect(set->prop_X2AxisVis(), &QcepBoolProperty::valueChanged, [=](bool vis) {setAxisVis(QwtPlot::xTop,    vis);});
    connect(set->prop_Y2AxisVis(), &QcepBoolProperty::valueChanged, [=](bool vis) {setAxisVis(QwtPlot::yRight,  vis);});
    connect(set->prop_XAxisLog(),  &QcepBoolProperty::valueChanged, [=](bool log) {setAxisLog(QwtPlot::xBottom, log);});
    connect(set->prop_YAxisLog(),  &QcepBoolProperty::valueChanged, [=](bool log) {setAxisLog(QwtPlot::yLeft,   log);});
    connect(set->prop_X2AxisLog(), &QcepBoolProperty::valueChanged, [=](bool log) {setAxisLog(QwtPlot::xTop,    log);});
    connect(set->prop_Y2AxisLog(), &QcepBoolProperty::valueChanged, [=](bool log) {setAxisLog(QwtPlot::yRight,  log);});
    connect(set->prop_LegendPosition(), &QcepIntProperty::valueChanged, [=](int pos) {setLegendPosition(pos);});

    setAxisVis(QwtPlot::xBottom, set->get_XAxisVis());
    setAxisVis(QwtPlot::yLeft,   set->get_YAxisVis());
    setAxisVis(QwtPlot::xTop,    set->get_X2AxisVis());
    setAxisVis(QwtPlot::yRight,  set->get_Y2AxisVis());
    setAxisLog(QwtPlot::xBottom, set->get_XAxisLog());
    setAxisLog(QwtPlot::yLeft,   set->get_YAxisLog());
    setAxisLog(QwtPlot::xTop,    set->get_X2AxisLog());
    setAxisLog(QwtPlot::yRight,  set->get_Y2AxisLog());
    setLegendPosition(set->get_LegendPosition());

    connect(m_Legend, &QwtLegend::clicked, this, &QcepPlotWidget::onLegendClicked);
    connect(m_Legend, &QwtLegend::checked, this, &QcepPlotWidget::onLegendChecked);
  }
}

QcepPlot *QcepPlotWidget::plot()
{
  return m_Plot;
}

void QcepPlotWidget::addPlotOverlay(QcepPlotOverlayPtr ovl)
{
  m_PlotOverlays.append(ovl);

  if (ovl) {
    ovl -> initialize(ovl->settings());
  }
}

void QcepPlotWidget::addPlotCommand(QcepPlotCommandPtr cmd)
{
  m_PlotCommands.append(cmd);

  if (cmd) {
    cmd -> initialize(cmd->settings());

    QToolButton *button = cmd->toolButton();

    if (button) {
      QLayout *layout = m_PlotTools->layout();

      if (layout) {
        layout->addWidget(button);
      }
    }
  }
}

void QcepPlotWidget::addPlotCommandSpacer()
{
  QSpacerItem *spacer =
      new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

  QLayout *layout = m_PlotTools->layout();

  if (layout) {
    layout->addItem(spacer);
  }
}

void QcepPlotWidget::contextMenu(const QPoint &pos)
{
  QMenu plotMenu(NULL, NULL);

  foreach (QcepPlotCommandPtr cmd, m_PlotCommands) {
    if (cmd) {
      QAction* act = cmd->contextMenuAction(pos);

      if (act) {
        plotMenu.addAction(act);
      }
    }
  }

  plotMenu.exec(mapToGlobal(pos));
}

void QcepPlotWidget::disableCommands(QcepPlotCommand* except)
{
  foreach (QcepPlotCommandPtr cmd, m_PlotCommands) {
    if (cmd && (cmd.data()!=except)) {
      cmd->disable();
    }
  }
}

void QcepPlotWidget::enableZooming()
{
  if (m_Zoomer) {
    m_Zoomer->setEnabled(true);
  }
}

void QcepPlotWidget::disableZooming()
{
  if (m_Zoomer) {
    m_Zoomer->setEnabled(false);
  }
}

void QcepPlotWidget::zoomIn()
{
  m_Zoomer -> zoom(1);
}

void QcepPlotWidget::zoomAll()
{
  m_Plot -> setAxisAutoScale(QwtPlot::yLeft);
  m_Plot -> setAxisAutoScale(QwtPlot::xBottom);

  m_Zoomer -> setZoomBase();
}

void QcepPlotWidget::zoomOut()
{
  m_Zoomer -> zoom(-1);
}

void QcepPlotWidget::printGraph()
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

void QcepPlotWidget::setLegendPosition(int legendPosition)
{
  if (legendPosition >= 0) {
    m_Plot -> insertLegend(m_Legend, (QwtPlot::LegendPosition) legendPosition);
  }
}


void QcepPlotWidget::onLegendClicked(const QVariant & /*itemInfo*/, int /*index*/)
{
//  if (g_Application) {
//    g_Application->printMessage(tr("QcepPlotWidget::onLegendClicked(%1,%2)").arg(itemInfo.toString()).arg(index));
//  }
}

void QcepPlotWidget::onLegendChecked(const QVariant &itemInfo, bool on, int /*index*/)
{
//  if (g_Application) {
//    g_Application->printMessage(tr("QcepPlotWidget::onLegendChecked(%1,%2,%3)").arg(itemInfo.toString()).arg(on).arg(index));
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

void QcepPlotWidget::setAxisVis(int axis, bool isVis)
{
  if (axis >= 0 && axis < QwtPlot::axisCnt) {
    m_Plot -> enableAxis(axis, isVis);
  }
}

void QcepPlotWidget::setAxisLog(int axis, bool isLog)
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

void QcepPlotWidget::updateTrackerPen(int mapIndex)
{
  QPen pen = QcepColorMapLibrary::trackerPen(mapIndex);

  m_Zoomer->setTrackerPen(pen);
  m_Zoomer->setRubberBandPen(pen);

  foreach (QcepPlotCommandPtr p, m_PlotCommands) {
    p->setPen(pen);
  }

  foreach (QcepPlotOverlayPtr o, m_PlotOverlays) {
    o->setPen(pen);
  }
}

void QcepPlotWidget::editPreferences()
{
  QcepPlotWidgetDialogPtr prefs =
      QcepPlotWidgetDialogPtr(
        new QcepPlotWidgetDialog(NULL, m_Settings));

  if (prefs) {
    prefs -> exec();
  }
}
