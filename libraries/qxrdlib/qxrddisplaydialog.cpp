#include "qxrddisplaydialog.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qxrdmainwindow.h"
#include "qxrdimageplotsettings-ptr.h"
#include "qxrdimageplotsettings.h"
#include "qxrdacquisition.h"
#include "qxrdimageplotwidget.h"

QxrdDisplayDialog::QxrdDisplayDialog(QWidget *parent,
                                     QxrdExperimentWPtr exptp,
                                     QxrdAcquisitionWPtr acqp,
                                     QxrdMainWindowWPtr winp,
                                     QxrdImagePlotWidgetWPtr plotp) :
    QDialog(parent),
    m_Experiment(exptp),
    m_Acquisition(acqp),
    m_MainWindow(winp),
    m_PlotWidget(plotp)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDisplayDialog::QxrdDisplayDialog(%p)\n", this);
  }

  setupUi(this);

  QxrdMainWindowPtr      win(m_MainWindow);
  QxrdImagePlotWidgetPtr wdg(m_PlotWidget);
  QxrdAcquisitionPtr     acq(m_Acquisition);

  if (win) {
    connect(m_DisplayOptionsButton, &QAbstractButton::clicked,
            win.data(), &QxrdMainWindow::doEditPreferences);
  }

  if (acq) {
    acq -> prop_OverflowLevel() -> linkTo(m_OverflowLevel);
  }

  if (wdg) {
    QxrdImagePlot *plot = wdg->m_Plot;

    if (plot) {
      connect(m_AutoRange, &QAbstractButton::clicked, plot, &QxrdImagePlot::setAutoRange);
      connect(m_Display_5pct, &QAbstractButton::clicked, plot, &QxrdImagePlot::set005Range);
      connect(m_Display_10pct, &QAbstractButton::clicked, plot, &QxrdImagePlot::set010Range);
      connect(m_Display_100pct, &QAbstractButton::clicked, plot, &QxrdImagePlot::set100Range);

      QxrdImagePlotSettingsPtr ps(plot->imagePlotSettings());

      if (ps) {
        ps -> prop_DisplayMinimumPct() -> linkTo(m_DisplayMinimumPct);
        ps -> prop_DisplayMaximumPct() -> linkTo(m_DisplayMaximumPct);
        ps -> prop_DisplayMinimumVal() -> linkTo(m_DisplayMinimumVal);
        ps -> prop_DisplayMaximumVal() -> linkTo(m_DisplayMaximumVal);
        ps -> prop_DisplayMinimumPctle() -> linkTo(m_DisplayMinimumPctle);
        ps -> prop_DisplayMaximumPctle() -> linkTo(m_DisplayMaximumPctle);

        ps -> prop_DisplayLog() -> linkTo(m_DisplayImageLog);
        ps -> prop_DisplayScalingMode() -> linkTo(m_DisplayScalingMode);

        connect(ps -> prop_DisplayScalingMode(), &QcepIntProperty::valueChanged, m_DisplayParmsStack, &QStackedWidget::setCurrentIndex);
        m_DisplayParmsStack->setCurrentIndex(ps->get_DisplayScalingMode());

        ps -> prop_DisplayColorMap() -> linkTo(m_DisplayColorMap);

        ps -> prop_ImageShown() -> linkTo(m_DisplayImage);
        ps -> prop_MaskShown() -> linkTo(m_DisplayMask);
        ps -> prop_OverflowShown() -> linkTo(m_DisplayOverflow);
        ps -> prop_InterpolatePixels() -> linkTo(m_InterpolatePixels);
        ps -> prop_MaintainAspectRatio() -> linkTo(m_MaintainAspectRatio);
      }
    }
  }
}

QxrdDisplayDialog::~QxrdDisplayDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDisplayDialog::~QxrdDisplayDialog(%p)\n", this);
  }
}

void QxrdDisplayDialog::changeEvent(QEvent *e)
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

void QxrdDisplayDialog::accept()
{
  QDialog::accept();
}
