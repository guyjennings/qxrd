#include "qxrdwatcherwindow.h"
#include "qxrdexperiment.h"
#include "qxrdwatcherwindowsettings.h"
#include "qxrdprocessor.h"
#include <QThread>

QxrdWatcherWindow::QxrdWatcherWindow(QxrdWatcherWindowSettingsWPtr set,
                                     QString name,
                                     QxrdAppCommonWPtr app,
                                     QxrdExperimentWPtr expt,
                                     QxrdAcqCommonWPtr acqw,
                                     QxrdProcessorWPtr procw,
                                     QxrdWatcherWPtr watcher) :
  inherited(name, app, expt, acqw, procw),
  m_WatcherWindowSettings(set),
  m_Watcher(watcher)
{
}

void QxrdWatcherWindow::initialize(QcepObjectWPtr parent)
{
  GUI_THREAD_CHECK;

  inherited::initialize(parent);

  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);

  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {    
    m_DatasetBrowserView -> setExperiment(exp);

    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdProcessorPtr     proc(m_Processor);
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
  inherited::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}
