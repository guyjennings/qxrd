#include "qxrdcenteringwindow.h"
#include "qxrdexperiment.h"
#include "qxrdcenteringwindowsettings.h"
#include "qxrdprocessor.h"
#include "qcepcenterfinder.h"
#include <QThread>

QxrdCenteringWindow::QxrdCenteringWindow(QString name) :
  inherited(name)
{
}

void QxrdCenteringWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);
  m_Splitter->setStretchFactor(2, 1);

  QxrdCenteringWindowSettingsPtr settings(
        qSharedPointerDynamicCast<QxrdCenteringWindowSettings>(m_Parent));

  QxrdExperimentPtr       exp(QxrdExperiment::findExperiment(m_Parent));
  QxrdDetectorSettingsPtr det(QxrdDetectorSettings::findDetectorSettings(m_Parent));
  QxrdProcessorPtr       proc(QxrdProcessor::findProcessor(m_Parent));
  QcepCenterFinderPtr      cf(QcepCenterFinder::findCenterFinder(m_Parent));

  if (cf == NULL) {
    if (proc) {
      cf = proc->centerFinder();
    } else if (det) {
      proc = det->processor();
      if (proc) {
        cf = proc->centerFinder();
      }
    } else if (exp) {
      proc = exp->processor();
      if (proc) {
        cf = proc->centerFinder();
      }
    }
  }

  if (exp) {
    m_DatasetBrowserView -> setExperiment(exp);

    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);


    if (settings) {
      m_FileBrowserWidget    -> initialize(settings->fileBrowserSettings(), exp, proc);
      m_ImagePlotWidget      -> initialize(settings->imagePlotWidgetSettings());
      m_ImagePlotWidget   -> setCenterFinder(cf);

      if (proc) {
        m_ImagePlotWidget   -> setMaskStack(proc->maskStack());
        m_ImagePlotWidget   -> setPowderRings(proc->powderRings());
        m_ImagePlotWidget   -> setROIModel(proc->roiModel());
      }

      m_ImagePlotWidget     -> addPlotCommandSpacer();

      m_CenteringPlotWidget  -> initialize(settings->centeringPlotWidgetSettings(), cf);
      m_IntegratedPlotWidget -> initialize(settings->integratedPlotWidgetSettings());
    } else {
      printMessage("No settings found for centering window");
    }

    if (cf) {
      cf->prop_Center()                -> linkTo(m_CenterX, m_CenterY);
      cf->prop_CenterStep()            -> linkTo(m_CenterStep);
      cf->prop_ImplementTilt()         -> linkTo(m_ImplementTilt);
      cf->prop_DetectorTilt()          -> linkTo(m_DetectorTilt);
      cf->prop_DetectorTiltStep()      -> linkTo(m_DetectorTiltStep);
      cf->prop_TiltPlaneRotation()     -> linkTo(m_TiltPlaneRotation);
      cf->prop_TiltPlaneRotationStep() -> linkTo(m_TiltPlaneRotationStep);

      CONNECT_CHECK(connect(m_CenterMoveDown,      &QToolButton::clicked, cf.data(), &QcepCenterFinder::centerMoveDown));
      CONNECT_CHECK(connect(m_CenterMoveUp,        &QToolButton::clicked, cf.data(), &QcepCenterFinder::centerMoveUp));
      CONNECT_CHECK(connect(m_CenterMoveLeft,      &QToolButton::clicked, cf.data(), &QcepCenterFinder::centerMoveLeft));
      CONNECT_CHECK(connect(m_CenterMoveRight,     &QToolButton::clicked, cf.data(), &QcepCenterFinder::centerMoveRight));
      CONNECT_CHECK(connect(m_CenterMoveDownLeft,  &QToolButton::clicked, cf.data(), &QcepCenterFinder::centerMoveDownLeft));
      CONNECT_CHECK(connect(m_CenterMoveDownRight, &QToolButton::clicked, cf.data(), &QcepCenterFinder::centerMoveDownRight));
      CONNECT_CHECK(connect(m_CenterMoveUpLeft,    &QToolButton::clicked, cf.data(), &QcepCenterFinder::centerMoveUpLeft));
      CONNECT_CHECK(connect(m_CenterMoveUpRight,   &QToolButton::clicked, cf.data(), &QcepCenterFinder::centerMoveUpRight));

      CONNECT_CHECK(
            connect(cf->prop_ImplementTilt(), &QcepBoolProperty::valueChanged,
                    this, &QxrdCenteringWindow::onImplementTiltChanged));

      onImplementTiltChanged(cf->get_ImplementTilt());

      CONNECT_CHECK(connect(cf->prop_CenterStep(),            &QcepDoubleProperty::valueChanged, this, &QxrdCenteringWindow::onStepSizesChanged));
      CONNECT_CHECK(connect(cf->prop_DetectorDistanceStep(),  &QcepDoubleProperty::valueChanged, this, &QxrdCenteringWindow::onStepSizesChanged));
      CONNECT_CHECK(connect(cf->prop_DetectorTiltStep(),      &QcepDoubleProperty::valueChanged, this, &QxrdCenteringWindow::onStepSizesChanged));
      CONNECT_CHECK(connect(cf->prop_TiltPlaneRotationStep(), &QcepDoubleProperty::valueChanged, this, &QxrdCenteringWindow::onStepSizesChanged));

      cf->prop_Energy()               -> linkTo(m_Energy);
      cf->prop_DetectorDistance()     -> linkTo(m_DetectorDistance);
      cf->prop_DetectorDistanceStep() -> linkTo(m_DetectorDistanceStep);
      cf->prop_DetectorXPixelSize()   -> linkTo(m_DetectorXPixelSize);
      cf->prop_DetectorYPixelSize()   -> linkTo(m_DetectorYPixelSize);
    } else {
      printMessage("No center finder found for centering window");
    }

    onStepSizesChanged();
  } else {
    printMessage("No experiment found for centering window");
  }
}

QxrdCenteringWindow::~QxrdCenteringWindow()
{
}

void QxrdCenteringWindow::changeEvent(QEvent *e)
{
  inherited::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdCenteringWindow::onStepSizesChanged()
{
  m_CenterX           -> setSingleStep(m_CenterStep->value());
  m_CenterY           -> setSingleStep(m_CenterStep->value());
  m_DetectorDistance  -> setSingleStep(m_DetectorDistanceStep->value());
  m_DetectorTilt      -> setSingleStep(m_DetectorTiltStep->value());
  m_TiltPlaneRotation -> setSingleStep(m_TiltPlaneRotationStep->value());
}

void QxrdCenteringWindow::onImplementTiltChanged(bool imp)
{
//  m_DetectorDistance  -> setEnabled(true /*imp*/);
  m_DetectorTilt          -> setEnabled(imp);
  m_DetectorTiltStep      -> setEnabled(imp);
  m_TiltPlaneRotation     -> setEnabled(imp);
  m_TiltPlaneRotationStep -> setEnabled(imp);
}
