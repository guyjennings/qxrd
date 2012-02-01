#include "qxrdexperimentpreferencesdialog.h"
#include "qxrdacquisition.h"
#include "qxrdacquisitionthread.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdallocator.h"
//#include "qxrdapplication.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>
#include "qcepdebug.h"

QxrdExperimentPreferencesDialog::QxrdExperimentPreferencesDialog(QxrdExperiment *doc, QWidget *parent) :
  QDialog(parent),
  m_Experiment(doc)
{
  setupUi(this);

//  connect(m_DetectorTypeCombo, SIGNAL(currentIndexChanged(int)), m_DetectorPrefsStack, SLOT(setCurrentIndex(int)));

  QxrdAcquisitionPtr acq = m_Experiment -> acquisition();
  QxrdDataProcessorPtr proc = m_Experiment->dataProcessor();
//  QxrdAllocator *alloc = g_Application->allocator();

  int detectorType = m_Experiment -> get_DetectorType();
//  int processorType = m_Experiment -> get_ProcessorType();

  int runSpecServer = m_Experiment -> get_RunSpecServer();
  int runSimpleServer = m_Experiment -> get_RunSimpleServer();
  int specServerPort = m_Experiment -> get_SpecServerPort();
  int simpleServerPort = m_Experiment -> get_SimpleServerPort();

//  QStringList detectorTypes = QxrdAcquisitionThread::detectorTypeNames();

//  m_DetectorTypeCombo -> addItems(detectorTypes);
//  m_DetectorTypeCombo -> setCurrentIndex(detectorType);

//  connect(m_CurrentOutputBrowse, SIGNAL(clicked()), this, SLOT(currentOutputBrowse()));
//  m_CurrentOutputDirectory -> setText(proc->get_OutputDirectory());

  connect(m_CurrentLogfileBrowse, SIGNAL(clicked()), this, SLOT(currentLogfileBrowse()));
  m_CurrentLogFile -> setText(m_Experiment->get_LogFileName());

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

  m_RunSpecServer -> setChecked(runSpecServer);
  m_RunSimpleServer -> setChecked(runSimpleServer);
  m_SpecServerPort -> setRange(-1,65535);
  m_SpecServerPort -> setSpecialValueText(tr("Automatic"));
  m_SpecServerPort -> setValue(specServerPort);
  m_SimpleServerPort -> setRange(0,65535);
  m_SimpleServerPort -> setValue(simpleServerPort);

  m_FileIndexWidth -> setValue(acq->get_FileIndexWidth());
  m_FilePhaseWidth -> setValue(acq->get_FilePhaseWidth());
  m_FileOverflowWidth -> setValue(acq->get_FileOverflowWidth());
}

QxrdExperimentPreferencesDialog::~QxrdExperimentPreferencesDialog()
{
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

void QxrdExperimentPreferencesDialog::currentLogfileBrowse()
{
  QDir pwd(m_CurrentOutputDirectory->text());
  QFileInfo initial(pwd, m_CurrentLogFile->text());

  QString file = QFileDialog::getSaveFileName(this, "Log File", initial.absoluteFilePath());

  if (file != "") {
    m_CurrentLogFile->setText(pwd.relativeFilePath(file));
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
  bool restartNeeded = false;

//  int detectorType = m_DetectorTypeCombo -> currentIndex();
//  int processorType = m_ProcessorTypeCombo -> currentIndex();
  int runSpecServer = m_RunSpecServer -> isChecked();
  int runSimpleServer = m_RunSimpleServer -> isChecked();
  int specServerPort = m_SpecServerPort -> value();
  int simpleServerPort = m_SimpleServerPort -> value();

//  if (detectorType != QxrdAcquisitionThread::detectorType()) {
//    restartNeeded = true;
//  }

//  if (processorType != QxrdDataProcessorThread::processorType()) {
//    restartNeeded = true;
//  }

  QxrdAcquisitionPtr acq = m_Experiment -> acquisition();
  QxrdDataProcessorPtr proc = m_Experiment->dataProcessor();

  if (runSpecServer != m_Experiment -> get_RunSpecServer()) {
    restartNeeded = true;
  }

  if (specServerPort != m_Experiment -> get_SpecServerPort()) {
    restartNeeded = true;
  }

  if (runSimpleServer != m_Experiment -> get_RunSimpleServer()) {
    restartNeeded = true;
  }

  if (simpleServerPort != m_Experiment -> get_SimpleServerPort()) {
    restartNeeded = true;
  }

  if (restartNeeded) {
    QMessageBox::information(this,"Restart Needed","You will need to restart qxrd before your changes will take effect");
  }

//  m_Experiment    -> set_DetectorType(detectorType);
//  app -> set_ProcessorType(processorType);
  m_Experiment    -> set_RunSpecServer(runSpecServer);
  m_Experiment    -> set_SpecServerPort(specServerPort);
  m_Experiment    -> set_RunSimpleServer(runSimpleServer);
  m_Experiment    -> set_SimpleServerPort(simpleServerPort);

  proc          -> set_SaveRawInSubdirectory(m_SaveRawInSubdir -> isChecked());
  proc          -> set_SaveRawSubdirectory  (m_SaveRawSubdir   -> text());

  proc          -> set_SaveDarkInSubdirectory(m_SaveDarkInSubdir  -> isChecked());
  proc          -> set_SaveDarkSubdirectory  (m_SaveDarkSubdir    -> text());

  proc          -> set_SaveSubtractedInSubdirectory(m_SaveSubtractedInSubdir -> isChecked());
  proc          -> set_SaveSubtractedSubdirectory  (m_SaveSubtractedSubdir   -> text());

  proc          -> set_SaveIntegratedData(m_SaveIntegratedInLogFile  -> isChecked());
  proc          -> set_SaveIntegratedInSeparateFiles(m_SaveIntegratedInSeparateFiles -> isChecked());
  proc          -> set_SaveIntegratedInSubdirectory (m_SaveIntegratedInSubdir  -> isChecked());
  proc          -> set_SaveIntegratedSubdirectory   (m_SaveIntegratedSubdir    -> text());

  proc          -> set_SaveOverflowFiles(m_SaveOverflowFiles -> isChecked());

//  proc          -> set_OutputDirectory(m_CurrentOutputDirectory -> text());
  m_Experiment    -> set_LogFileName    (m_CurrentLogFile -> text());

  acq           -> set_FileIndexWidth(m_FileIndexWidth -> value());
  acq           -> set_FilePhaseWidth(m_FilePhaseWidth -> value());
  acq           -> set_FileOverflowWidth(m_FileOverflowWidth -> value());

  QDialog::accept();
}

