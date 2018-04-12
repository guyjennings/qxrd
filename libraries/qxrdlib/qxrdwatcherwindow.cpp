#include "qxrdwatcherwindow.h"
#include "qxrdexperiment.h"
#include "qxrdwatcherwindowsettings.h"
#include "qxrdprocessor.h"
#include <QThread>

QxrdWatcherWindow::QxrdWatcherWindow(QString name) :
  inherited(name),
  m_Watcher()
{
}

void QxrdWatcherWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  setupUi(this);

  inherited::initialize(parent);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);

  QxrdExperimentPtr exp(QxrdExperiment::findExperiment(m_Parent));

  if (exp) {    
    m_DatasetBrowserView -> setExperiment(exp);

    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdProcessorPtr     proc(QxrdProcessor::findProcessor(m_Parent));
    QcepCenterFinderPtr  cf(proc?proc->centerFinder():QcepCenterFinderWPtr());

    QxrdWatcherWindowSettingsPtr settings(
          qSharedPointerDynamicCast<QxrdWatcherWindowSettings>(m_Parent));

    if (settings) {
      m_FileBrowserWidget   -> initialize(settings->fileBrowserSettings(), exp, proc);
      m_ImagePlotWidget     -> initialize(settings->imagePlotWidgetSettings());
      m_ImagePlotWidget     -> setCenterFinder(cf);

      if (proc) {
        m_ImagePlotWidget   -> setMaskStack(proc->maskStack());
        m_ImagePlotWidget   -> setPowderRings(proc->powderRings());
        m_ImagePlotWidget   -> setROIModel(proc->roiModel());
      }

      m_CenteringPlotWidget -> initialize(settings->centeringPlotWidgetSettings(), cf);
    }
  }
}

QxrdWatcherWindow::~QxrdWatcherWindow()
{
}

void QxrdWatcherWindow::changeEvent(QEvent *e)
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
