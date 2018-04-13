#include "qcepimageplotwidgetdialog.h"
#include "qcepdebug.h"
#include <stdio.h>
#include "qcepimageplotsettings-ptr.h"
#include "qcepimageplotsettings.h"
#include "qcepimageplotwidget.h"
#include "qcepimageplotwidgetsettings.h"
#include <QComboBox>
#include "qcepcolormaplibrary.h"

QcepImagePlotWidgetDialog::QcepImagePlotWidgetDialog(QWidget *parent,
                                     QcepImagePlotWidgetSettingsWPtr settings) :
    QDialog(parent),
    m_Settings(settings)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepImagePlotWidgetDialog::QcepImagePlotWidgetDialog(%p)\n", this);
  }

  setupUi(this);

  QcepImagePlotWidgetSettingsPtr set(m_Settings);

  if (set) {
    set->prop_XAxisVis()            -> copyTo(m_XAxisVis);
    set->prop_YAxisVis()            -> copyTo(m_YAxisVis);
    set->prop_X2AxisVis()           -> copyTo(m_X2AxisVis);
    set->prop_Y2AxisVis()           -> copyTo(m_Y2AxisVis);

    set->prop_XAxisLog()            -> copyTo(m_XAxisLog);
    set->prop_YAxisLog()            -> copyTo(m_YAxisLog);
    set->prop_X2AxisLog()           -> copyTo(m_X2AxisLog);
    set->prop_Y2AxisLog()           -> copyTo(m_Y2AxisLog);

    set->prop_LegendPosition()      -> copyTo(m_LegendPosition);

    set->prop_DisplayMinimumPctle() -> copyTo(m_DisplayMinimumPctle);
    set->prop_DisplayMaximumPctle() -> copyTo(m_DisplayMaximumPctle);
    set->prop_DisplayMinimumPct()   -> copyTo(m_DisplayMinimumPct);
    set->prop_DisplayMaximumPct()   -> copyTo(m_DisplayMaximumPct);
    set->prop_DisplayMinimumVal()   -> copyTo(m_DisplayMinimumVal);
    set->prop_DisplayMaximumVal()   -> copyTo(m_DisplayMaximumVal);
    set->prop_DisplayScalingMode()  -> copyTo(m_DisplayScalingMode);

    connect(m_Display_5pct, &QPushButton::clicked,    [=]() { setMaxPct(5); } );
    connect(m_Display_10pct, &QPushButton::clicked,   [=]() { setMaxPct(10); } );
    connect(m_Display_100pct, &QPushButton::clicked,  [=]() { setMaxPct(100); } );

    connect(m_Display_90pctle, &QPushButton::clicked, [=]() { setMaxPctle(90); } );
    connect(m_Display_95pctle, &QPushButton::clicked, [=]() { setMaxPctle(95); } );
    connect(m_Display_99pctle, &QPushButton::clicked, [=]() { setMaxPctle(99); } );

    connect(m_DisplayScalingMode, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged,
            m_DisplayParmsStack,  &QStackedWidget::setCurrentIndex);

    m_DisplayParmsStack->setCurrentIndex(set->get_DisplayScalingMode());

    for (int i=0; i<QcepColorMapLibrary::colorMapCount(); i++) {
      m_DisplayColorMap->addItem(QcepColorMapLibrary::colorMapName(i));
    }

    set->prop_DisplayColorMap()     -> copyTo(m_DisplayColorMap);

    set->prop_ImageShown()          -> copyTo(m_DisplayImage);
    set->prop_MaskShown()           -> copyTo(m_DisplayMask);
    set->prop_OverflowShown()       -> copyTo(m_DisplayOverflow);
    set->prop_InterpolatePixels()   -> copyTo(m_InterpolatePixels);
    set->prop_MaintainAspectRatio() -> copyTo(m_MaintainAspectRatio);
    set->prop_DisplayROI()          -> copyTo(m_DisplayROI);
    set->prop_DisplayLog()          -> copyTo(m_DisplayImageLog);

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
//  }
  }
}

QcepImagePlotWidgetDialog::~QcepImagePlotWidgetDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepImagePlotWidgetDialog::~QcepImagePlotWidgetDialog(%p)\n", this);
  }
}

void QcepImagePlotWidgetDialog::setMaxPct(double v)
{
  m_DisplayMaximumPct->setValue(v);
}

void QcepImagePlotWidgetDialog::setMaxPctle(double v)
{
  m_DisplayMaximumPctle->setValue(v);
}

void QcepImagePlotWidgetDialog::changeEvent(QEvent *e)
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

void QcepImagePlotWidgetDialog::accept()
{
  QcepImagePlotWidgetSettingsPtr set(m_Settings);

  if (set) {
    set->prop_XAxisVis()            -> copyFrom(m_XAxisVis);
    set->prop_YAxisVis()            -> copyFrom(m_YAxisVis);
    set->prop_X2AxisVis()           -> copyFrom(m_X2AxisVis);
    set->prop_Y2AxisVis()           -> copyFrom(m_Y2AxisVis);

    set->prop_XAxisLog()            -> copyFrom(m_XAxisLog);
    set->prop_YAxisLog()            -> copyFrom(m_YAxisLog);
    set->prop_X2AxisLog()           -> copyFrom(m_X2AxisLog);
    set->prop_Y2AxisLog()           -> copyFrom(m_Y2AxisLog);

    set->prop_LegendPosition()      -> copyFrom(m_LegendPosition);

    set->prop_DisplayMinimumPctle() -> copyFrom(m_DisplayMinimumPctle);
    set->prop_DisplayMaximumPctle() -> copyFrom(m_DisplayMaximumPctle);
    set->prop_DisplayMinimumPct()   -> copyFrom(m_DisplayMinimumPct);
    set->prop_DisplayMaximumPct()   -> copyFrom(m_DisplayMaximumPct);
    set->prop_DisplayMinimumVal()   -> copyFrom(m_DisplayMinimumVal);
    set->prop_DisplayMaximumVal()   -> copyFrom(m_DisplayMaximumVal);
    set->prop_DisplayScalingMode()  -> copyFrom(m_DisplayScalingMode);

    set->prop_DisplayColorMap()     -> copyFrom(m_DisplayColorMap);

    set->prop_ImageShown()          -> copyFrom(m_DisplayImage);
    set->prop_MaskShown()           -> copyFrom(m_DisplayMask);
    set->prop_OverflowShown()       -> copyFrom(m_DisplayOverflow);
    set->prop_InterpolatePixels()   -> copyFrom(m_InterpolatePixels);
    set->prop_MaintainAspectRatio() -> copyFrom(m_MaintainAspectRatio);
    set->prop_DisplayROI()          -> copyFrom(m_DisplayROI);
    set->prop_DisplayLog()          -> copyFrom(m_DisplayImageLog);
  }

  QDialog::accept();
}
