#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qxrdmainwindow.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdacqcommon.h"
#include "qxrdmainwindowsettings-ptr.h"
#include "qxrdmainwindowsettings.h"
#include "qxrdexperimentpreferencesdialog.h"
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QFileDialog>
#include <QThread>
#include "qcepallocator.h"

QxrdMainWindow::QxrdMainWindow(QString name)
  : QcepMainWindow(),
    m_Name(name)
{
}

void QxrdMainWindow::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QxrdMainWindow::setupMenus(QMenu *file, QMenu *edit, QMenu *window)
{
  inherited::setupMenus(file, edit, window);

  QxrdAcqCommonPtr acq(QxrdAcqCommon::findAcquisition(m_Parent));

  if (acq) {
    connect(acq.data(), &QxrdAcqCommon::acquireStarted,
            this,       &QxrdMainWindow::acquireStarted);

    connect(acq.data(), &QxrdAcqCommon::acquiredFrame,
            this,       &QxrdMainWindow::acquiredFrame);

    connect(acq.data(), &QxrdAcqCommon::acquireComplete,
            this,       &QxrdMainWindow::acquireComplete);
  }
}

void QxrdMainWindow::populateWindowsMenu()
{
  inherited::populateWindowsMenu();

  QxrdExperimentPtr expt(
        QxrdExperiment::findExperiment(m_Parent));

  QxrdAppCommonPtr app(QxrdAppCommon::findApplication(m_Parent));

  if (expt == NULL) {
    expt = app->experiment(0);

    int nWin = expt->windowSettingsCount();

    for (int i=0; i<nWin; i++) {
      appendToWindowMenu(m_WindowMenuP,
                         expt -> windowSettings(i));
    }
  }

  if (expt) {
    QxrdAcqCommonPtr acqp(expt->acquisition());

    QxrdAcqCommon *acq = acqp.data();

    if (acq) {
      int nWin = acq->windowSettingsCount();

      for (int i=0; i<nWin; i++) {
        appendToWindowMenu(m_WindowMenuP,
                           acq -> windowSettings(i));
      }

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

        connect(act, &QAction::triggered,
                [=]() {acq->configureDetector(i);});

        configMenu -> addAction(act);

        QString str2 =
            tr("(%1) Open %2 detector \"%3\" Control...")
            .arg(i).arg(detType).arg(detName);

        QAction *act2 =
            new QAction(str2);

        act2 -> setCheckable(true);
        act2 -> setChecked(enabled);

        connect(act2, &QAction::triggered,
                [=]() {acq->openDetectorControlWindow(i);});

        ctrlMenu -> addAction(act2);
      }

      acquireWins   -> addMenu(configMenu);
      acquireWins   -> addMenu(ctrlMenu);

      m_WindowMenuP -> insertMenu(
            m_WindowMenuP -> actions().first(),
            acquireWins);
    }
  }
}

void QxrdMainWindow::updateTitle()
{
  QxrdExperimentPtr exper(QxrdExperiment::findExperiment(m_Parent));

  QString title;

  title = m_Name;

  if (exper) {
    title.append(" - ");
    title.append(exper->experimentFilePath());

    exper -> prop_CompletionPercentage() -> linkTo(m_Progress);
  }

  title.append(" - QXRD");

  if (sizeof(void*) == 4) {
    title.append(" - 32 bit - v");
  } else {
    title.append(" - 64 bit - v");
  }

  title.append(STR(QXRD_VERSION));

  if (exper && exper->isChanged()) {
    title.append(tr(" [%1]").arg(exper->isChanged()));
  }

  setWindowTitle(title);
}

void QxrdMainWindow::printLine(QString /*line*/)
{
}

void QxrdMainWindow::printMessage(QString /*msg*/, QDateTime /*ts*/)
{
}

void QxrdMainWindow::criticalMessage(QString /*msg*/, QDateTime /*ts*/)
{
}

void QxrdMainWindow::statusMessage(QString /*msg*/, QDateTime /*ts*/)
{
}

void QxrdMainWindow::acquireStarted()
{
}

void QxrdMainWindow::acquireComplete()
{
  THREAD_CHECK;

  m_Progress -> reset();
}

void QxrdMainWindow::acquiredFrame(
    QString fileName, int iphase, int nphases, int isum, int nsum, int igroup, int ngroup)
{
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

  m_Progress -> setValue(thisFrame*100/totalFrames);
}

void QxrdMainWindow::doEditPreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr exp(QxrdExperiment::findExperiment(m_Parent));

  QxrdExperimentPreferencesDialog prefs(exp, NULL, 1);

  prefs.exec();
}

void QxrdMainWindow::doEditDetectorPreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr exp(QxrdExperiment::findExperiment(m_Parent));

  QxrdExperimentPreferencesDialog prefs(exp, NULL, 0);

  prefs.exec();
}

void QxrdMainWindow::saveExperimentAs()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(QxrdExperiment::findExperiment(m_Parent));
  QxrdAppCommonPtr  app(QxrdAppCommon::findApplication(m_Parent));

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

  QxrdExperimentPtr expt(QxrdExperiment::findExperiment(m_Parent));
  QxrdAppCommonPtr  app(QxrdAppCommon::findApplication(m_Parent));

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

