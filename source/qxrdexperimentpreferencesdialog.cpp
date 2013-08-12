#include "qxrddebug.h"
#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdacquisition.h"
#include "qxrdacquisitionthread.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdallocator.h"
#include "qxrdexperiment.h"
#include "qxrdserver.h"
#include "qxrdsimpleserver.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>
#include "qcepdebug.h"

QxrdExperimentPreferencesDialog::QxrdExperimentPreferencesDialog(QxrdExperimentWPtr exptw, QWidget *parent) :
  QDialog(parent),
  m_Experiment(exptw)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentPreferencesDialog::QxrdExperimentPreferencesDialog(%p)\n", this);
  }

  setupUi(this);

  QxrdExperimentPtr expt(m_Experiment);

//  connect(m_DetectorTypeCombo, SIGNAL(currentIndexChanged(int)), m_DetectorPrefsStack, SLOT(setCurrentIndex(int)));

  if (expt) {
    setWindowTitle(expt->experimentFilePath() + " Preferences");

    QxrdAcquisitionPtr acq = expt -> acquisition();
    QxrdDataProcessorPtr proc = expt->dataProcessor();
    //  QxrdAllocator *alloc = g_Application->allocator();

//    int detectorType = expt -> get_DetectorType();
    //  int processorType = m_Experiment -> get_ProcessorType();

    int runSpecServer = 0;
    int specServerPort = 0;

    QxrdServerPtr srv(expt -> specServer());

    if (srv) {
      runSpecServer = srv -> get_RunSpecServer();
      specServerPort = srv -> get_SpecServerPort();
    }

    int runSimpleServer = 0;
    int simpleServerPort = 0;

    QxrdSimpleServerPtr ssrv(expt -> simpleServer());

    if (ssrv) {
      runSimpleServer = ssrv -> get_RunSimpleServer();
      simpleServerPort = ssrv -> get_SimpleServerPort();
    }

    //  QStringList detectorTypes = QxrdAcquisitionThread::detectorTypeNames();

    //  m_DetectorTypeCombo -> addItems(detectorTypes);
    //  m_DetectorTypeCombo -> setCurrentIndex(detectorType);

    connect(m_CurrentOutputBrowse, SIGNAL(clicked()), this, SLOT(currentOutputBrowse()));
    m_CurrentOutputDirectory -> setText(expt->get_ExperimentDirectory());

    connect(m_CurrentLogFileBrowse, SIGNAL(clicked()), this, SLOT(currentLogFileBrowse()));
    m_CurrentLogFile -> setText(expt->get_LogFileName());

    connect(m_CurrentScanFileBrowse, SIGNAL(clicked()), this, SLOT(currentScanFileBrowse()));
    m_CurrentScanFile -> setText(expt->get_ScanFileName());

    if (proc) {
      connect(m_SaveRawBrowse, SIGNAL(clicked()), this, SLOT(saveRawBrowse()));
      m_SaveRawInSubdir  -> setChecked(proc->get_SaveRawInSubdirectory());
      m_SaveRawSubdir    -> setText  (proc->get_SaveRawSubdirectory());

      connect(m_SaveDarkBrowse, SIGNAL(clicked()), this, SLOT(saveDarkBrowse()));
      m_SaveDarkInSubdir  -> setChecked(proc->get_SaveDarkInSubdirectory());
      m_SaveDarkSubdir    -> setText  (proc->get_SaveDarkSubdirectory());

      connect(m_SaveSubtractedBrowse, SIGNAL(clicked()), this, SLOT(saveSubtractedBrowse()));
      m_SaveSubtractedInSubdir  -> setChecked(proc->get_SaveSubtractedInSubdirectory());
      m_SaveSubtractedSubdir    -> setText  (proc->get_SaveSubtractedSubdirectory());

      connect(m_SaveIntegratedBrowse, SIGNAL(clicked()), this, SLOT(saveIntegratedBrowse()));
      m_SaveIntegratedInLogFile  -> setChecked(proc->get_SaveIntegratedData());
      m_SaveIntegratedInSeparateFiles  -> setChecked(proc->get_SaveIntegratedInSeparateFiles());
      m_SaveIntegratedInSubdir  -> setChecked(proc->get_SaveIntegratedInSubdirectory());
      m_SaveIntegratedSubdir    -> setText  (proc->get_SaveIntegratedSubdirectory());

      m_SaveOverflowFiles -> setChecked(proc->get_SaveOverflowFiles());
    }

    if (srv) {
      m_RunSpecServer -> setChecked(runSpecServer);
      m_SpecServerPort -> setRange(-1,65535);
      m_SpecServerPort -> setSpecialValueText(tr("Automatic"));
      m_SpecServerPort -> setValue(specServerPort);
    }

    if (ssrv) {
      m_RunSimpleServer -> setChecked(runSimpleServer);
      m_SimpleServerPort -> setRange(0,65535);
      m_SimpleServerPort -> setValue(simpleServerPort);
    }

    if (acq) {
      m_FileIndexWidth -> setValue(acq->get_FileIndexWidth());
      m_FilePhaseWidth -> setValue(acq->get_FilePhaseWidth());
      m_FileOverflowWidth -> setValue(acq->get_FileOverflowWidth());
    }
  }
}

QxrdExperimentPreferencesDialog::~QxrdExperimentPreferencesDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentPreferencesDialog::~QxrdExperimentPreferencesDialog(%p)\n", this);
  }
}

void QxrdExperimentPreferencesDialog::getRelativeDirectoryPath(QLineEdit *edit)
{
  QDir pwd(m_CurrentOutputDirectory->text());
  QFileInfo initial(pwd, edit->text());

  QString dir = QFileDialog::getExistingDirectory(this, "", initial.absolutePath(), QFileDialog::ShowDirsOnly);

  if (dir != "") {
    edit -> setText(pwd.relativeFilePath(dir));
  }
}

void QxrdExperimentPreferencesDialog::saveRawBrowse()
{
  getRelativeDirectoryPath(m_SaveRawSubdir);
}

void QxrdExperimentPreferencesDialog::saveDarkBrowse()
{
  getRelativeDirectoryPath(m_SaveDarkSubdir);
}

void QxrdExperimentPreferencesDialog::saveSubtractedBrowse()
{
  getRelativeDirectoryPath(m_SaveSubtractedSubdir);
}

void QxrdExperimentPreferencesDialog::saveIntegratedBrowse()
{
  getRelativeDirectoryPath(m_SaveIntegratedSubdir);
}

void QxrdExperimentPreferencesDialog::currentOutputBrowse()
{
  QString dir = QFileDialog::getExistingDirectory(this, "Output Directory", m_CurrentOutputDirectory->text(), QFileDialog::ShowDirsOnly);

  if (dir != "") {
    m_CurrentOutputDirectory->setText(dir);
  }
}

void QxrdExperimentPreferencesDialog::currentLogFileBrowse()
{
  QDir pwd(m_CurrentOutputDirectory->text());
  QFileInfo initial(pwd, m_CurrentLogFile->text());

  QString file = QFileDialog::getSaveFileName(this, "Log File", initial.absoluteFilePath());

  if (file != "") {
    m_CurrentLogFile->setText(pwd.relativeFilePath(file));
  }
}

void QxrdExperimentPreferencesDialog::currentScanFileBrowse()
{
  QDir pwd(m_CurrentOutputDirectory->text());
  QFileInfo initial(pwd, m_CurrentScanFile->text());

  QString file = QFileDialog::getSaveFileName(this, "Scan File", initial.absoluteFilePath());

  if (file != "") {
    m_CurrentScanFile->setText(pwd.relativeFilePath(file));
  }
}

void QxrdExperimentPreferencesDialog::changeEvent(QEvent *e)
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

void QxrdExperimentPreferencesDialog::accept()
{
  int runSpecServer = m_RunSpecServer -> isChecked();
  int runSimpleServer = m_RunSimpleServer -> isChecked();
  int specServerPort = m_SpecServerPort -> value();
  int simpleServerPort = m_SimpleServerPort -> value();

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdAcquisitionPtr acq = expt -> acquisition();
    QxrdDataProcessorPtr proc = expt->dataProcessor();
    QxrdServerPtr srv(expt -> specServer());
    QxrdSimpleServerPtr ssrv(expt -> simpleServer());

    if (srv) {
      srv -> set_RunSpecServer(runSpecServer);
      srv -> set_SpecServerPort(specServerPort);
    }

    if (ssrv) {
      ssrv -> set_RunSimpleServer(runSimpleServer);
      ssrv -> set_SimpleServerPort(simpleServerPort);
    }

    if (proc) {
      proc -> set_SaveRawInSubdirectory(m_SaveRawInSubdir -> isChecked());
      proc -> set_SaveRawSubdirectory  (m_SaveRawSubdir   -> text());

      proc -> set_SaveDarkInSubdirectory(m_SaveDarkInSubdir  -> isChecked());
      proc -> set_SaveDarkSubdirectory  (m_SaveDarkSubdir    -> text());

      proc -> set_SaveSubtractedInSubdirectory(m_SaveSubtractedInSubdir -> isChecked());
      proc -> set_SaveSubtractedSubdirectory  (m_SaveSubtractedSubdir   -> text());

      proc -> set_SaveIntegratedData(m_SaveIntegratedInLogFile  -> isChecked());
      proc -> set_SaveIntegratedInSeparateFiles(m_SaveIntegratedInSeparateFiles -> isChecked());
      proc -> set_SaveIntegratedInSubdirectory (m_SaveIntegratedInSubdir  -> isChecked());
      proc -> set_SaveIntegratedSubdirectory   (m_SaveIntegratedSubdir    -> text());

      proc -> set_SaveOverflowFiles(m_SaveOverflowFiles -> isChecked());
    }

    expt -> set_ExperimentDirectory(m_CurrentOutputDirectory -> text());
    expt -> set_LogFileName    (m_CurrentLogFile -> text());
    expt -> set_ScanFileName    (m_CurrentScanFile -> text());

    if (acq) {
      acq  -> set_FileIndexWidth(m_FileIndexWidth -> value());
      acq  -> set_FilePhaseWidth(m_FilePhaseWidth -> value());
      acq  -> set_FileOverflowWidth(m_FileOverflowWidth -> value());
    }
  }

  QDialog::accept();
}

