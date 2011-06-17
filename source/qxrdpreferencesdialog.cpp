#include "qxrdpreferencesdialog.h"
#include "qxrdacquisition.h"
#include "qxrdacquisitionthread.h"
#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdallocator.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QGridLayout>
#include "qcepdebug.h"

QxrdPreferencesDialog::QxrdPreferencesDialog(QWidget *parent) :
    QDialog(parent)
{
  setupUi(this);

  connect(m_DetectorTypeCombo, SIGNAL(currentIndexChanged(int)), m_DetectorPrefsStack, SLOT(setCurrentIndex(int)));

  QxrdApplication *app = QxrdApplication::application();
  QxrdAcquisition *acq = app -> acquisition();
  QxrdDataProcessor *proc = app->dataProcessor();
  QxrdAllocator *alloc = app->allocator();

  int detectorType = app -> get_DetectorType();
//  int processorType = app -> get_ProcessorType();
  int debugLevel = app -> get_Debug();

  int runSpecServer = app -> get_RunSpecServer();
  int runSimpleServer = app -> get_RunSimpleServer();
  int specServerPort = app -> get_SpecServerPort();
  int simpleServerPort = app -> get_SimpleServerPort();

  QStringList detectorTypes = QxrdAcquisitionThread::detectorTypeNames();

  m_DetectorTypeCombo -> addItems(detectorTypes);
  m_DetectorTypeCombo -> setCurrentIndex(detectorType);

  connect(m_CurrentOutputBrowse, SIGNAL(clicked()), this, SLOT(currentOutputBrowse()));
  m_CurrentOutputDirectory -> setText(proc->get_OutputDirectory());

  connect(m_CurrentLogfileBrowse, SIGNAL(clicked()), this, SLOT(currentLogfileBrowse()));
  m_CurrentLogFile -> setText(app->get_LogFilePath());

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

  m_ReservedMemory32 -> setRange(500, 3000);
  m_ReservedMemory32 -> setValue(acq->get_TotalBufferSizeMB32());
  m_ReservedMemory64 -> setRange(500, 60000);
  m_ReservedMemory64 -> setValue(acq->get_TotalBufferSizeMB64());
  m_ExtraReservedMemory -> setRange(0, 500);
  m_ExtraReservedMemory -> setValue(alloc->get_Reserve());

  m_RunSpecServer -> setChecked(runSpecServer);
  m_RunSimpleServer -> setChecked(runSimpleServer);
  m_SpecServerPort -> setRange(-1,65535);
  m_SpecServerPort -> setSpecialValueText(tr("Automatic"));
  m_SpecServerPort -> setValue(specServerPort);
  m_SimpleServerPort -> setRange(0,65535);
  m_SimpleServerPort -> setValue(simpleServerPort);

  m_FileBrowserLimit -> setValue(app->get_FileBrowserLimit());

  setupDebugWidgets(debugLevel);
}

QxrdPreferencesDialog::~QxrdPreferencesDialog()
{
}

void QxrdPreferencesDialog::getRelativeDirectoryPath(QLineEdit *edit)
{
  QDir pwd(m_CurrentOutputDirectory->text());
  QFileInfo initial(pwd, edit->text());

  QString dir = QFileDialog::getExistingDirectory(this, "", initial.absolutePath(), QFileDialog::ShowDirsOnly);

  if (dir != "") {
    edit -> setText(pwd.relativeFilePath(dir));
  }
}

void QxrdPreferencesDialog::saveRawBrowse()
{
  getRelativeDirectoryPath(m_SaveRawSubdir);
}

void QxrdPreferencesDialog::saveDarkBrowse()
{
  getRelativeDirectoryPath(m_SaveDarkSubdir);
}

void QxrdPreferencesDialog::saveSubtractedBrowse()
{
  getRelativeDirectoryPath(m_SaveSubtractedSubdir);
}

void QxrdPreferencesDialog::saveIntegratedBrowse()
{
  getRelativeDirectoryPath(m_SaveIntegratedSubdir);
}

void QxrdPreferencesDialog::currentOutputBrowse()
{
  QString dir = QFileDialog::getExistingDirectory(this, "Output Directory", m_CurrentOutputDirectory->text(), QFileDialog::ShowDirsOnly);

  if (dir != "") {
    m_CurrentOutputDirectory->setText(dir);
  }
}

void QxrdPreferencesDialog::currentLogfileBrowse()
{
  QDir pwd(m_CurrentOutputDirectory->text());
  QFileInfo initial(pwd, m_CurrentLogFile->text());

  QString file = QFileDialog::getSaveFileName(this, "Log File", initial.absoluteFilePath());

  if (file != "") {
    m_CurrentLogFile->setText(pwd.relativeFilePath(file));
  }
}

void QxrdPreferencesDialog::changeEvent(QEvent *e)
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

void QxrdPreferencesDialog::accept()
{
  bool restartNeeded = false;

  int detectorType = m_DetectorTypeCombo -> currentIndex();
//  int processorType = m_ProcessorTypeCombo -> currentIndex();
  int debugLevel = readDebugWidgets();
  int bufferSize32 = m_ReservedMemory32 -> value();
  int bufferSize64 = m_ReservedMemory64 -> value();
  int extraReserve = m_ExtraReservedMemory -> value();
  int runSpecServer = m_RunSpecServer -> isChecked();
  int runSimpleServer = m_RunSimpleServer -> isChecked();
  int specServerPort = m_SpecServerPort -> value();
  int simpleServerPort = m_SimpleServerPort -> value();

  if (detectorType != QxrdAcquisitionThread::detectorType()) {
    restartNeeded = true;
  }

//  if (processorType != QxrdDataProcessorThread::processorType()) {
//    restartNeeded = true;
//  }

  QxrdApplication *app = QxrdApplication::application();
  QxrdAcquisition *acq = app -> acquisition();
  QxrdDataProcessor *proc = app->dataProcessor();
  QxrdAllocator *alloc = app->allocator();

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
  alloc -> set_Reserve(extraReserve);
  app -> set_RunSpecServer(runSpecServer);
  app -> set_SpecServerPort(specServerPort);
  app -> set_RunSimpleServer(runSimpleServer);
  app -> set_SimpleServerPort(simpleServerPort);

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

  proc -> set_OutputDirectory(m_CurrentOutputDirectory -> text());
  app -> set_LogFilePath    (m_CurrentLogFile -> text());

  app -> set_FileBrowserLimit(m_FileBrowserLimit->value());

  QDialog::accept();
}

void QxrdPreferencesDialog::setupDebugWidgets(int dbg)
{
  QGridLayout *grid = new QGridLayout(m_DebugWidgets);

  int mask = 1;

  for (int i=0; gDebugStrings[i]; i++) {
    QCheckBox *cb = new QCheckBox(gDebugStrings[i]);
    cb->setChecked(dbg & mask);

    mask <<= 1;

    m_DebugWidgetList.append(cb);
  }

  int ndebug = m_DebugWidgetList.count();
  int ncol   = ndebug - ndebug/2;

  for (int i=0; gDebugStrings[i]; i++) {
    QCheckBox *cb = m_DebugWidgetList[i];

    if (i < ncol) {
      grid->addWidget(cb, i, 0);
    } else {
      grid->addWidget(cb, i - ncol, 1);
    }
  }
}

int QxrdPreferencesDialog::readDebugWidgets()
{
  int mask = 1;
  int newDbg = 0;

  for (int i=0; gDebugStrings[i]; i++) {
    if (m_DebugWidgetList[i]->isChecked()) {
      newDbg |= mask;
    }

    mask <<= 1;
  }

  return newDbg;
}
