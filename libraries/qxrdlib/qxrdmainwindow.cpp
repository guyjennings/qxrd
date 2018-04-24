#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qxrdmainwindow.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdacqcommon.h"
#include "qxrdmainwindowsettings-ptr.h"
#include "qxrdmainwindowsettings.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdexposurepreferencesdialog.h"
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QFileDialog>
#include <QThread>
#include "qcepallocator.h"

QxrdMainWindow::QxrdMainWindow(QString name)
  : inherited(name)
{
}

void QxrdMainWindow::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  QxrdExperimentPtr exper(qSharedPointerDynamicCast<QxrdExperiment>(findExperiment()));

  if (exper) {
    exper -> prop_CompletionPercentage() -> linkTo(m_Progress);
  }
}

void QxrdMainWindow::setupMenus(QMenu *file, QMenu *edit, QMenu *window)
{
  inherited::setupMenus(file, edit, window);

  QxrdAcqCommonPtr acq(QxrdAcqCommon::findAcquisition(m_Parent));

  if (acq) {
    CONNECT_CHECK(
          connect(acq.data(), &QxrdAcqCommon::acquireStarted,
                  this,       &QxrdMainWindow::acquireStarted));

    CONNECT_CHECK(
          connect(acq.data(), &QxrdAcqCommon::acquiredFrame,
                  this,       &QxrdMainWindow::acquiredFrame));

    CONNECT_CHECK(
          connect(acq.data(), &QxrdAcqCommon::acquireComplete,
                  this,       &QxrdMainWindow::acquireComplete));
  }
}

void QxrdMainWindow::populateWindowsMenu()
{
  inherited::populateWindowsMenu();

  QxrdExperimentPtr  expt(
        qSharedPointerDynamicCast<QxrdExperiment>(findExperiment()));

  QcepApplicationPtr app(findApplication());

  if (app) {
    if (expt == NULL) {
      expt = qSharedPointerDynamicCast<QxrdExperiment>(app->experiment(0));

      if (expt) {
        int nWin = expt->windowSettingsCount();

        for (int i=0; i<nWin; i++) {
          appendToWindowMenu(m_WindowMenuP,
                             expt -> windowSettings(i));
        }
      }
    }

    int nAppWin = app->windowSettingsCount();

    for (int i=0; i<nAppWin; i++) {
      appendToWindowMenu(m_WindowMenuP,
                         app->windowSettings(i));
    }
  }

  if (expt) {
    QxrdAcqCommonPtr acqp(expt->acquisition());

    QxrdAcqCommon *acq = acqp.data();

    if (acq) {
      QMenu *acquireWins = new QMenu("Detectors");

      acquireWins -> addAction("Setup Detectors...",
                               this, &QxrdMainWindow::doEditDetectorPreferences);

      QMenu *configMenu = new QMenu("Configure Detector");
      QMenu *ctrlMenu   = new QMenu("Open Detector Window");

      int nDets = acq->detectorCount();

      for (int i=0; i<nDets; i++) {
        QxrdDetectorSettingsPtr det = acq->detector(i);

        QString detType = det->get_DetectorTypeName();
        QString detName = det->get_DetectorName();
        bool    enabled = det->get_Enabled();

        QString str =
            tr("(%1) Configure %2 \"%3\"...")
            .arg(i).arg(detType).arg(detName);

        QAction *act =
            new QAction(str);

        act -> setCheckable(true);
        act -> setChecked(enabled);

        CONNECT_CHECK(
              connect(act, &QAction::triggered,
                      [=]() {acq->configureDetector(i);}));

        configMenu -> addAction(act);

        QString str2 =
            tr("(%1) Open %2 \"%3\" Control...")
            .arg(i).arg(detType).arg(detName);

        QAction *act2 =
            new QAction(str2);

        act2 -> setCheckable(true);
        act2 -> setChecked(enabled);

        CONNECT_CHECK(
              connect(act2, &QAction::triggered,
                      [=]() {acq->openDetectorControlWindow(i);}));

        ctrlMenu -> addAction(act2);
      }

      acquireWins   -> addMenu(configMenu);
      acquireWins   -> addMenu(ctrlMenu);

      QMenu* detsMenu = new QMenu("Detectors");

      for (int i=0; i<nDets; i++) {
        QxrdDetectorSettingsPtr det = acq->detector(i);

        if (det) {
          int nWin = det->windowSettingsCount();

          if (nWin > 0) {
            QMenu* detMenu = new QMenu(tr("Detector %1").arg(i));

            for (int i=0; i<nWin; i++) {
              appendToWindowMenu(detMenu,
                                 det->windowSettings(i));
            }

            detsMenu -> addMenu(detMenu);
          }
        }
      }

      acquireWins -> addMenu(detsMenu);

      int nWin = acq->windowSettingsCount();

      for (int i=0; i<nWin; i++) {
        appendToWindowMenu(acquireWins,
                           acq -> windowSettings(i));
      }

      m_WindowMenuP -> insertMenu(
            m_WindowMenuP -> actions().first(),
            acquireWins);
    }
  }
}

//void QxrdMainWindow::updateTitle()
//{
//  QxrdExperimentPtr exper(qSharedPointerDynamicCast<QxrdExperiment>(findExperiment()));

//  QString title = m_Name;

//  if (exper) {
//    title.append(" - ");
//    title.append(exper->experimentFilePath());
//  }

//  title.append(" - QXRD");

//  if (sizeof(void*) == 4) {
//    title.append(" - 32 bit - v");
//  } else {
//    title.append(" - 64 bit - v");
//  }

//  title.append(g_Application->applicationVersion());

//  if (exper && exper->isChanged()) {
//    title.append(tr(" [%1]").arg(exper->isChanged()));
//  }

//  setWindowTitle(title);
//}

void QxrdMainWindow::acquireStarted()
{
}

void QxrdMainWindow::acquireComplete()
{
}

void QxrdMainWindow::acquiredFrame(
    QString fileName,
    int iphase,
    int nphases,
    int isum,
    int nsum,
    int igroup,
    int ngroup)
{
  THREAD_CHECK;

  int totalFrames = (nphases*nsum*ngroup <= 0 ? 1 : nphases*nsum*ngroup);
  int thisFrame   = igroup*nphases*nsum + isum*nphases + iphase + 1;

  if (nphases <= 1) {
    displayStatusMessage(tr("%1: Exposure %2 of %3, File %4 of %5")
                         .arg(fileName)
                         .arg(isum+1).arg(nsum)
                         .arg(igroup+1).arg(ngroup));
  } else {
    displayStatusMessage(tr("%1: Phase %2 of %3, Sum %4 of %5, Group %6 of %7")
                         .arg(fileName)
                         .arg(iphase+1).arg(nphases)
                         .arg(isum+1).arg(nsum)
                         .arg(igroup+1).arg(ngroup));
  }
}

void QxrdMainWindow::doEditPreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr exp(qSharedPointerDynamicCast<QxrdExperiment>(findExperiment()));

  QxrdExperimentPreferencesDialog prefs(exp, NULL, 1);

  prefs.exec();
}

void QxrdMainWindow::doEditExposurePreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr exp(qSharedPointerDynamicCast<QxrdExperiment>(findExperiment()));

  if (exp) {
    QxrdAcqCommonPtr acq(exp->acquisition());

    if (acq) {
      QxrdExposurePreferencesDialog prefs(acq);

      prefs.exec();
    }
  }
}
void QxrdMainWindow::doEditDetectorPreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr exp(qSharedPointerDynamicCast<QxrdExperiment>(findExperiment()));

  QxrdExperimentPreferencesDialog prefs(exp, NULL, 0);

  prefs.exec();
}

void QxrdMainWindow::saveExperiment()
{
  QxrdExperimentPtr expt(qSharedPointerDynamicCast<QxrdExperiment>(findExperiment()));

  if (expt) {
    expt->saveExperiment();
  }
}

void QxrdMainWindow::saveExperimentAs()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(qSharedPointerDynamicCast<QxrdExperiment>(findExperiment()));
  QxrdAppCommonPtr  app(qSharedPointerDynamicCast<QxrdAppCommon>(findApplication()));

  if (app && expt) {
    QString path = expt->experimentFilePath();
    QString name = expt->defaultExperimentName(path);
    QString dirp = expt->defaultExperimentDirectory(path);

    QDir dir(expt->get_ExperimentDirectory());

    QString newPath = dir.filePath(name+"-copy.qxrdp");

    QString newChoice = QFileDialog::getSaveFileName(NULL,
                                                     "Save Experiment As",
                                                     newPath,
                                                     "QXRD Experiments (*.qxrdp)");

    if (newChoice.length()>0) {
      expt->saveExperimentAs(newChoice);
      app->appendRecentExperiment(newChoice);
    }
  }
}

void QxrdMainWindow::saveExperimentCopy()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(qSharedPointerDynamicCast<QxrdExperiment>(findExperiment()));
  QxrdAppCommonPtr  app(qSharedPointerDynamicCast<QxrdAppCommon>(findApplication()));

  if (app && expt) {
    QString path = expt->experimentFilePath();
    QString name = expt->defaultExperimentName(path);
//    QString dirp = expt->defaultExperimentDirectory(path);

    QDir dir(expt->get_ExperimentDirectory());

    QString newPath = dir.filePath(name+"-copy.qxrdp");

    QString newChoice = QFileDialog::getSaveFileName(NULL,
                                                     "Save Experiment Copy",
                                                     newPath,
                                                     "QXRD Experiments (*.qxrdp)");

    if (newChoice.length()>0) {
      expt->saveExperimentCopyAs(newChoice);
      app->appendRecentExperiment(newChoice);
    }
  }
}

