#include "qxrdcenteringwindow.h"
#include "qxrdexperiment.h"
#include "qxrdcenteringwindowsettings.h"
#include "qxrddataprocessor.h"

QxrdCenteringWindow::QxrdCenteringWindow(QxrdCenteringWindowSettingsWPtr set,
                                         QString name,
                                         QxrdApplicationWPtr app,
                                         QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acqw, QxrdProcessorWPtr procw) :
  QxrdMainWindow(name, app, expt, acqw, procw),
  m_CenteringWindowSettings(set)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);
  m_Splitter->setStretchFactor(2, 1);

  m_DatasetBrowserView -> setExperiment(expt);

  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdDataProcessorPtr proc(exp->dataProcessor());
    QxrdCenterFinderPtr  cf(proc?proc->centerFinder():QxrdCenterFinderWPtr());

    QxrdCenteringWindowSettingsPtr settings(m_CenteringWindowSettings);

    if (settings) {
      m_FileBrowserWidget    -> initialize(settings->fileBrowserSettings(), exp, proc);
      m_ImagePlotWidget      -> initialize(settings->imagePlotWidgetSettings(), proc);
      m_CenteringPlotWidget  -> initialize(settings->centeringPlotWidgetSettings(), cf);
      m_IntegratedPlotWidget -> initialize(settings->integratedPlotWidgetSettings());
    }

    if (cf) {
      cf->prop_Center()                -> linkTo(m_CenterX, m_CenterY);
      cf->prop_CenterStep()            -> linkTo(m_CenterStep);
      cf->prop_ImplementTilt()         -> linkTo(m_ImplementTilt);
      cf->prop_DetectorTilt()          -> linkTo(m_DetectorTilt);
      cf->prop_DetectorTiltStep()      -> linkTo(m_DetectorTiltStep);
      cf->prop_TiltPlaneRotation()     -> linkTo(m_TiltPlaneRotation);
      cf->prop_TiltPlaneRotationStep() -> linkTo(m_TiltPlaneRotationStep);

      connect(m_CenterMoveDown,      &QToolButton::clicked, cf.data(), &QxrdCenterFinder::centerMoveDown);
      connect(m_CenterMoveUp,        &QToolButton::clicked, cf.data(), &QxrdCenterFinder::centerMoveUp);
      connect(m_CenterMoveLeft,      &QToolButton::clicked, cf.data(), &QxrdCenterFinder::centerMoveLeft);
      connect(m_CenterMoveRight,     &QToolButton::clicked, cf.data(), &QxrdCenterFinder::centerMoveRight);
      connect(m_CenterMoveDownLeft,  &QToolButton::clicked, cf.data(), &QxrdCenterFinder::centerMoveDownLeft);
      connect(m_CenterMoveDownRight, &QToolButton::clicked, cf.data(), &QxrdCenterFinder::centerMoveDownRight);
      connect(m_CenterMoveUpLeft,    &QToolButton::clicked, cf.data(), &QxrdCenterFinder::centerMoveUpLeft);
      connect(m_CenterMoveUpRight,   &QToolButton::clicked, cf.data(), &QxrdCenterFinder::centerMoveUpRight);
    }
  }
}

QxrdCenteringWindow::~QxrdCenteringWindow()
{
}

void QxrdCenteringWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
