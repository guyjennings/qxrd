#include "qxrdwatcherwindow.h"
#include "qxrdexperiment.h"
#include "qxrdwatcherwindowsettings.h"
#include "qxrddataprocessor.h"

QxrdWatcherWindow::QxrdWatcherWindow(QxrdWatcherWindowSettingsWPtr set,
                                     QString name,
                                     QxrdAppCommonWPtr app,
                                     QxrdExperimentWPtr expt,
                                     QxrdAcquisitionWPtr acqw,
                                     QxrdProcessorWPtr procw,
                                     QxrdWatcherWPtr watcher) :
  QxrdMainWindow(name, app, expt, acqw, procw),
  m_WatcherWindowSettings(set),
  m_Watcher(watcher)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);

  m_DatasetBrowserView -> setExperiment(expt);

  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdDataProcessorPtr proc(exp->dataProcessor());
    QxrdCenterFinderPtr  cf(proc?proc->centerFinder():QxrdCenterFinderWPtr());

    QxrdWatcherWindowSettingsPtr settings(m_WatcherWindowSettings);

    if (settings) {
      m_FileBrowserWidget   -> initialize(settings->fileBrowserSettings(), exp, proc);
      m_ImagePlotWidget     -> initialize(settings->imagePlotWidgetSettings(), proc);
      m_CenteringPlotWidget -> initialize(settings->centeringPlotWidgetSettings(), cf);
    }
  }
}

QxrdWatcherWindow::~QxrdWatcherWindow()
{
}

void QxrdWatcherWindow::changeEvent(QEvent *e)
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
