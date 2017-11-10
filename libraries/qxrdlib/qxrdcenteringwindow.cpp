#include "qxrdcenteringwindow.h"
#include "qxrdexperiment.h"
#include "qxrdcenteringwindowsettings.h"

QxrdCenteringWindow::QxrdCenteringWindow(QxrdCenteringWindowSettingsWPtr set,
                                         QString name,
                                         QxrdApplicationWPtr app,
                                         QxrdExperimentWPtr expt) :
  QxrdMainWindow(name, app, expt),
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

    QxrdCenteringWindowSettingsPtr settings(m_CenteringWindowSettings);

    if (settings) {
      m_FileBrowserWidget -> init(settings->fileBrowserSettings(), exp, proc);
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
