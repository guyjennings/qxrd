#include "qcepplotwidgetdialog.h"
#include "qcepdebug.h"
#include <stdio.h>
#include "qcepimageplotsettings-ptr.h"
#include "qcepimageplotsettings.h"
#include "qcepimageplotwidget.h"
#include "qcepplotwidgetsettings.h"

QcepPlotWidgetDialog::QcepPlotWidgetDialog(QWidget *parent,
                                     QcepPlotWidgetSettingsWPtr settings) :
    QDialog(parent),
    m_Settings(settings)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepPlotWidgetDialog::QcepPlotWidgetDialog(%p)\n", this);
  }

  setupUi(this);

  QcepPlotWidgetSettingsPtr set(m_Settings);

  if (set) {
    set->prop_XAxisVis()->copyTo(m_XAxisVis);
    set->prop_YAxisVis()->copyTo(m_YAxisVis);
    set->prop_X2AxisVis()->copyTo(m_X2AxisVis);
    set->prop_Y2AxisVis()->copyTo(m_Y2AxisVis);

    set->prop_XAxisLog()->copyTo(m_XAxisLog);
    set->prop_YAxisLog()->copyTo(m_YAxisLog);
    set->prop_X2AxisLog()->copyTo(m_X2AxisLog);
    set->prop_Y2AxisLog()->copyTo(m_Y2AxisLog);

    set->prop_LegendPosition()->copyTo(m_LegendPosition);

    //  QcepMainWindowPtr      win(m_MainWindow);
//  QcepImagePlotWidgetPtr wdg(m_PlotWidget);
//  QcepAcquisitionPtr     acq(m_Acquisition);

//  if (win) {
//    connect(m_DisplayOptionsButton, &QAbstractButton::clicked,
//            win.data(), &QcepMainWindow::doEditPreferences);
//  }

//  if (acq) {
//    acq -> prop_OverflowLevel() -> linkTo(m_OverflowLevel);
//  }

//  if (wdg) {
//    QcepImagePlot *plot = wdg->m_Plot;

//    if (plot) {
//      connect(m_AutoRange, &QAbstractButton::clicked, plot, &QcepImagePlot::setAutoRange);
//      connect(m_Display_5pct, &QAbstractButton::clicked, plot, &QcepImagePlot::set005Range);
//      connect(m_Display_10pct, &QAbstractButton::clicked, plot, &QcepImagePlot::set010Range);
//      connect(m_Display_100pct, &QAbstractButton::clicked, plot, &QcepImagePlot::set100Range);

//      QcepImagePlotSettingsPtr ps(plot->imagePlotSettings());

//      if (ps) {
//        ps -> prop_DisplayMinimumPct() -> linkTo(m_DisplayMinimumPct);
//        ps -> prop_DisplayMaximumPct() -> linkTo(m_DisplayMaximumPct);
//        ps -> prop_DisplayMinimumVal() -> linkTo(m_DisplayMinimumVal);
//        ps -> prop_DisplayMaximumVal() -> linkTo(m_DisplayMaximumVal);
//        ps -> prop_DisplayMinimumPctle() -> linkTo(m_DisplayMinimumPctle);
//        ps -> prop_DisplayMaximumPctle() -> linkTo(m_DisplayMaximumPctle);

//        ps -> prop_DisplayLog() -> linkTo(m_DisplayImageLog);
//        ps -> prop_DisplayScalingMode() -> linkTo(m_DisplayScalingMode);

//        connect(ps -> prop_DisplayScalingMode(), &QcepIntProperty::valueChanged, m_DisplayParmsStack, &QStackedWidget::setCurrentIndex);
//        m_DisplayParmsStack->setCurrentIndex(ps->get_DisplayScalingMode());

//        ps -> prop_DisplayColorMap() -> linkTo(m_DisplayColorMap);

//        ps -> prop_ImageShown() -> linkTo(m_DisplayImage);
//        ps -> prop_MaskShown() -> linkTo(m_DisplayMask);
//        ps -> prop_OverflowShown() -> linkTo(m_DisplayOverflow);
//        ps -> prop_InterpolatePixels() -> linkTo(m_InterpolatePixels);
//        ps -> prop_MaintainAspectRatio() -> linkTo(m_MaintainAspectRatio);
//      }
//    }
  }
}

QcepPlotWidgetDialog::~QcepPlotWidgetDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepPlotWidgetDialog::~QcepPlotWidgetDialog(%p)\n", this);
  }
}

void QcepPlotWidgetDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void QcepPlotWidgetDialog::accept()
{
  QDialog::accept();

  QcepPlotWidgetSettingsPtr set(m_Settings);

  if (set) {
    set->prop_XAxisVis()->copyFrom(m_XAxisVis);
    set->prop_YAxisVis()->copyFrom(m_YAxisVis);
    set->prop_X2AxisVis()->copyFrom(m_X2AxisVis);
    set->prop_Y2AxisVis()->copyFrom(m_Y2AxisVis);

    set->prop_XAxisLog()->copyFrom(m_XAxisLog);
    set->prop_YAxisLog()->copyFrom(m_YAxisLog);
    set->prop_X2AxisLog()->copyFrom(m_X2AxisLog);
    set->prop_Y2AxisLog()->copyFrom(m_Y2AxisLog);

    set->prop_LegendPosition()->copyFrom(m_LegendPosition);
  }
}
