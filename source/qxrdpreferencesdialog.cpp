#include "qxrdpreferencesdialog.h"
#include "ui_qxrdpreferencesdialog.h"
#include "qxrdacquisitionthread.h"
#include "qxrddataprocessorthread.h"
#include "qxrdapplication.h"
#include <QMessageBox>

QxrdPreferencesDialog::QxrdPreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QxrdPreferencesDialog)
{
  ui->setupUi(this);

  connect(ui -> m_DetectorTypeCombo, SIGNAL(currentIndexChanged(int)), ui->m_DetectorPrefsStack, SLOT(setCurrentIndex(int)));

  QxrdSettings settings;
  QxrdApplication *app = QxrdApplication::application();
  QxrdAcquisition *acq = app -> acquisition();

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

  QDialog::accept();
}
