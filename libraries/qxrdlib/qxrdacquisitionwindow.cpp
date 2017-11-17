#include "qxrdacquisitionwindow.h"
#include "qxrdexperiment.h"
#include "qxrdacquisitionwindowsettings.h"
#include "qxrdcorrectiondialog.h"

QxrdAcquisitionWindow::QxrdAcquisitionWindow(QxrdAcquisitionWindowSettingsWPtr set,
                                             QString name,
                                             QxrdApplicationWPtr app,
                                             QxrdExperimentWPtr expt) :
  QxrdMainWindow(name, app, expt),
  m_AcquisitionWindowSettings(set)
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

    QxrdAcquisitionWindowSettingsPtr settings(m_AcquisitionWindowSettings);

    if (settings) {
      m_FileBrowserWidget -> init(settings->fileBrowserSettings(), exp, proc);
    }
  }

  connect(m_AcquireOptionsButton, &QPushButton::clicked,
          this, &QxrdAcquisitionWindow::doEditCorrection);
}

QxrdAcquisitionWindow::~QxrdAcquisitionWindow()
{
}

void QxrdAcquisitionWindow::changeEvent(QEvent *e)
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

void QxrdAcquisitionWindow::doEditCorrection()
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QxrdAcquisitionPtr   acq  = exp->acquisition();
    QxrdDataProcessorPtr proc = exp->dataProcessor();

    if (acq && proc) {
      QxrdCorrectionDialog* editCorrection =
          new QxrdCorrectionDialog(this, acq, proc);

      editCorrection->exec();
    }
  }
}
