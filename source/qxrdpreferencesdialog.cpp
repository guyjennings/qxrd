#include "qxrdpreferencesdialog.h"
#include "ui_qxrdpreferencesdialog.h"
#include "qxrdacquisitionthread.h"
#include "qxrddataprocessorthread.h"
#include "qxrdapplication.h"
#include <QMessageBox>
#include <QFileDialog>

QxrdPreferencesDialog::QxrdPreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QxrdPreferencesDialog)
{
  ui->setupUi(this);

  connect(ui -> m_DetectorTypeCombo, SIGNAL(currentIndexChanged(int)), ui->m_DetectorPrefsStack, SLOT(setCurrentIndex(int)));

  QxrdSettings settings;
  QxrdApplication *app = QxrdApplication::application();
  QxrdAcquisition *acq = app -> acquisition();
  QxrdDataProcessor *proc = app->dataProcessor();

  int detectorType = app -> get_DetectorType();
  int processorType = app -> get_ProcessorType();
  int debugLevel = app -> get_Debug();

  int runSpecServer = app -> get_RunSpecServer();
  int runSimpleServer = app -> get_RunSimpleServer();
  int specServerPort = app -> get_SpecServerPort();
  int simpleServerPort = app -> get_SimpleServerPort();

  QStringList detectorTypes = QxrdAcquisitionThread::detectorTypeNames();

  ui -> m_DetectorTypeCombo -> addItems(detectorTypes);
  ui -> m_DetectorTypeCombo -> setCurrentIndex(detectorType);

  connect(ui->m_CurrentOutputBrowse, SIGNAL(clicked()), this, SLOT(currentOutputBrowse()));
  ui ->m_CurrentOutputDirectory -> setText(proc->get_OutputDirectory());

  connect(ui->m_CurrentLogfileBrowse, SIGNAL(clicked()), this, SLOT(currentLogfileBrowse()));
  ui ->m_CurrentLogFile -> setText(proc->get_LogFilePath());

  connect(ui->m_SaveRawBrowse, SIGNAL(clicked()), this, SLOT(saveRawBrowse()));
  ui -> m_SaveRawInSubdir  -> setChecked(acq->get_SaveRawInSubdirectory());
  ui -> m_SaveRawSubdir    -> setText  (acq->get_SaveRawSubdirectory());

  connect(ui->m_SaveDarkBrowse, SIGNAL(clicked()), this, SLOT(saveDarkBrowse()));
  ui -> m_SaveDarkInSubdir  -> setChecked(acq->get_SaveDarkInSubdirectory());
  ui -> m_SaveDarkSubdir    -> setText  (acq->get_SaveDarkSubdirectory());

  connect(ui->m_SaveSubtractedBrowse, SIGNAL(clicked()), this, SLOT(saveSubtractedBrowse()));
  ui -> m_SaveSubtractedInSubdir  -> setChecked(acq->get_SaveSubtractedInSubdirectory());
  ui -> m_SaveSubtractedSubdir    -> setText  (acq->get_SaveSubtractedSubdirectory());

  connect(ui->m_SaveIntegratedBrowse, SIGNAL(clicked()), this, SLOT(saveIntegratedBrowse()));
  ui -> m_SaveIntegratedSeparately  -> setChecked(acq->get_SaveIntegratedInSeparateFiles());
  ui -> m_SaveIntegratedInSubdir  -> setChecked(acq->get_SaveIntegratedInSubdirectory());
  ui -> m_SaveIntegratedSubdir    -> setText  (acq->get_SaveIntegratedSubdirectory());

  ui -> m_DebugLevelSpinBox -> setRange(0,65535);
  ui -> m_DebugLevelSpinBox -> setValue(debugLevel);

  ui -> m_ReservedMemory32 -> setRange(500, 3000);
  ui -> m_ReservedMemory32 -> setValue(acq->get_TotalBufferSizeMB32());
  ui -> m_ReservedMemory64 -> setRange(500, 60000);
  ui -> m_ReservedMemory64 -> setValue(acq->get_TotalBufferSizeMB64());

  ui -> m_RunSpecServer -> setChecked(runSpecServer);
  ui -> m_RunSimpleServer -> setChecked(runSimpleServer);
  ui -> m_SpecServerPort -> setRange(-1,65535);
  ui -> m_SpecServerPort -> setSpecialValueText(tr("Automatic"));
  ui -> m_SpecServerPort -> setValue(specServerPort);
  ui -> m_SimpleServerPort -> setRange(0,65535);
  ui -> m_SimpleServerPort -> setValue(simpleServerPort);
}

QxrdPreferencesDialog::~QxrdPreferencesDialog()
{
  delete ui;
}

void QxrdPreferencesDialog::getRelativeDirectoryPath(QLineEdit *edit)
{
  QDir pwd(ui ->m_CurrentOutputDirectory->text());
  QFileInfo initial(pwd, edit->text());

  QString dir = QFileDialog::getExistingDirectory(this, "", initial.absolutePath(), QFileDialog::ShowDirsOnly);

  if (dir != "") {
    edit -> setText(pwd.relativeFilePath(dir));
  }
}

void QxrdPreferencesDialog::saveRawBrowse()
{
  getRelativeDirectoryPath(ui->m_SaveRawSubdir);
}

void QxrdPreferencesDialog::saveDarkBrowse()
{
  getRelativeDirectoryPath(ui->m_SaveDarkSubdir);
}

void QxrdPreferencesDialog::saveSubtractedBrowse()
{
  getRelativeDirectoryPath(ui->m_SaveSubtractedSubdir);
}

void QxrdPreferencesDialog::saveIntegratedBrowse()
{
  getRelativeDirectoryPath(ui->m_SaveIntegratedSubdir);
}

void QxrdPreferencesDialog::currentOutputBrowse()
{
  QString dir = QFileDialog::getExistingDirectory(this, "Output Directory", ui->m_CurrentOutputDirectory->text(), QFileDialog::ShowDirsOnly);

  if (dir != "") {
    ui->m_CurrentOutputDirectory->setText(dir);
  }
}

void QxrdPreferencesDialog::currentLogfileBrowse()
{
  QDir pwd(ui ->m_CurrentOutputDirectory->text());
  QFileInfo initial(pwd, ui->m_CurrentLogFile->text());

  QString file = QFileDialog::getSaveFileName(this, "Log File", initial.absoluteFilePath());

  if (file != "") {
    ui->m_CurrentLogFile->setText(pwd.relativeFilePath(file));
  }
}

void QxrdPreferencesDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdPreferencesDialog::accept()
{
  int detectorType = ui -> m_DetectorTypeCombo -> currentIndex();
//  int processorType = ui -> m_ProcessorTypeCombo -> currentIndex();
  int debugLevel = ui -> m_DebugLevelSpinBox -> value();
  int bufferSize32 = ui -> m_ReservedMemory32 -> value();
  int bufferSize64 = ui -> m_ReservedMemory64 -> value();
  int runSpecServer = ui -> m_RunSpecServer -> isChecked();
  int runSimpleServer = ui -> m_RunSimpleServer -> isChecked();
  int specServerPort = ui -> m_SpecServerPort -> value();
  int simpleServerPort = ui -> m_SimpleServerPort -> value();

  bool restartNeeded = false;

  if (detectorType != QxrdAcquisitionThread::detectorType()) {
    restartNeeded = true;
  }

//  if (processorType != QxrdDataProcessorThread::processorType()) {
//    restartNeeded = true;
//  }

  QxrdApplication *app = QxrdApplication::application();
  QxrdAcquisition *acq = app -> acquisition();
  QxrdDataProcessor *proc = app->dataProcessor();

  if (runSpecServer != app -> get_RunSpecServer()) {
    restartNeeded = true;
  }

  if (specServerPort != app -> get_SpecServerPort()) {
    restartNeeded = true;
  }

  if (runSimpleServer != app -> get_RunSimpleServer()) {
    restartNeeded = true;
  }

  if (simpleServerPort != app -> get_SimpleServerPort()) {
    restartNeeded = true;
  }

  if (restartNeeded) {
    QMessageBox::information(this,"Restart Needed","You will need to restart qxrd before your changes will take effect");
  }

  app -> set_DetectorType(detectorType);
//  app -> set_ProcessorType(processorType);
  app -> set_Debug(debugLevel);
  acq -> set_TotalBufferSizeMB32(bufferSize32);
  acq -> set_TotalBufferSizeMB64(bufferSize64);
  app -> set_RunSpecServer(runSpecServer);
  app -> set_SpecServerPort(specServerPort);
  app -> set_RunSimpleServer(runSimpleServer);
  app -> set_SimpleServerPort(simpleServerPort);

  acq -> set_SaveRawInSubdirectory(ui -> m_SaveRawInSubdir -> isChecked());
  acq -> set_SaveRawSubdirectory  (ui -> m_SaveRawSubdir   -> text());

  acq -> set_SaveDarkInSubdirectory(ui -> m_SaveDarkInSubdir  -> isChecked());
  acq -> set_SaveDarkSubdirectory  (ui -> m_SaveDarkSubdir    -> text());

  acq -> set_SaveSubtractedInSubdirectory(ui -> m_SaveSubtractedInSubdir -> isChecked());
  acq -> set_SaveSubtractedSubdirectory  (ui -> m_SaveSubtractedSubdir   -> text());

  acq -> set_SaveIntegratedInSeparateFiles(ui -> m_SaveIntegratedSeparately -> isChecked());
  acq -> set_SaveIntegratedInSubdirectory (ui -> m_SaveIntegratedInSubdir  -> isChecked());
  acq -> set_SaveIntegratedSubdirectory   (ui -> m_SaveIntegratedSubdir    -> text());

  proc-> set_OutputDirectory(ui -> m_CurrentOutputDirectory -> text());
  proc-> set_LogFilePath    (ui -> m_CurrentLogFile -> text());

  QDialog::accept();
}
