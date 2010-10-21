/******************************************************************
*
*  $Id: qxrdpreferencesdialog.cpp,v 1.3 2010/10/21 19:44:03 jennings Exp $
*
*******************************************************************/

#include "qxrdpreferencesdialog.h"
#include "ui_qxrdpreferencesdialog.h"
#include "qxrdacquisitionthread.h"
#include "qxrddataprocessorthread.h"
#include "qxrdapplication.h"
#include <QMessageBox>

QxrdPreferencesDialog::QxrdPreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QxrdPreferencesDialog),
    SOURCE_IDENT("$Id: qxrdpreferencesdialog.cpp,v 1.3 2010/10/21 19:44:03 jennings Exp $")
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
  ui -> m_ReservedMemory -> setRange(100, 20000);
  ui -> m_ReservedMemory -> setValue(acq->get_TotalBufferSizeMB());

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
  int bufferSize = ui -> m_ReservedMemory -> value();
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
  acq -> set_TotalBufferSizeMB(bufferSize);
  app -> set_RunSpecServer(runSpecServer);
  app -> set_SpecServerPort(specServerPort);
  app -> set_RunSimpleServer(runSimpleServer);
  app -> set_SimpleServerPort(simpleServerPort);

  QDialog::accept();
}

/******************************************************************
*
*  $Log: qxrdpreferencesdialog.cpp,v $
*  Revision 1.3  2010/10/21 19:44:03  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/09/09 16:45:29  jennings
*  Always use threaded data processor
*
*  Revision 1.1.2.2  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.4  2010/05/27 21:47:51  jennings
*  Renamed totalBufferSize parameter to totalBufferSizeMB to avoid conflicts with qxrd 0.3.x which interprets
*  totalBufferSize in bytes, rather than MB
*
*  Revision 1.1.2.3  2010/05/21 20:02:15  jennings
*  Adopt tabbed format for preferences dialog, with sub pages for different detector and processor types
*
*  Revision 1.1.2.2  2010/05/21 18:30:32  jennings
*  Implemented preferences dialog allowing choise of detector type, processor type and debug level
*
*  Revision 1.1.2.1  2010/04/21 19:48:41  jennings
*  Added QxrdAcquisitionAreaDetector, QxrdAcquisitionPilatus, QxrdDataProcessorCuda, QxrdDataProcessorThreaded
*  and QxrdPreferencesDialog - they don't do much yet, though.
*
*
*******************************************************************/

