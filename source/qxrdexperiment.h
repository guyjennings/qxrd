#ifndef QXRDEXPERIMENT_H
#define QXRDEXPERIMENT_H

#include "qcepmacros.h"
#include "qcepproperty.h"

#include <QObject>
#include <QSharedPointer>
#include <QSplashScreen>
#include "qxrddataprocessorthread.h"
#include "qxrdacquisitionthread.h"
#include "qxrdserverthread.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdfilesaverthread.h"
#include "qxrdsettingssaverthread.h"

class QxrdWindow;
class QxrdNIDAQPluginInterface;

class QxrdExperiment : public QObject
{
  Q_OBJECT

public:
  QxrdExperiment(QString path, QxrdApplication *app, QSettings *settings, QObject *parent = 0);
  virtual bool init(QSettings *settings);
  virtual ~QxrdExperiment();
  void closeExperiment();

  QxrdAcquisitionThread *acquisitionThread();
  QxrdAcquisition *acquisition() const;
  QxrdWindow *window();
  QxrdDataProcessor *dataProcessor() const;

  FILE* logFile();
  void newLogFile(QString path);

  FILE* scanFile();
  void newScanFile(QString path);

  QxrdSettingsSaver *saver();

  QString defaultDirectory(QString path);
  QString defaultFileName(QString path);
  QString defaultExperimentName(QString path);
  QString defaultLogName(QString path);
  QString defaultScanName(QString path);

  QString experimentFilePath();
  QString logFilePath();
  QString scanFilePath();

  void saveExperiment();
  void saveExperimentCopy();

signals:

public slots:
  void readSettings();
  void writeSettings();
  virtual void readSettings(QSettings *settings, QString section="experiment");
  virtual void writeSettings(QSettings *settings, QString section="experiment");

  void splashMessage(const char *msg);
  void splashMessage(QString msg);
  void criticalMessage(QString msg);
  void statusMessage(QString msg);
  void printMessage(QString msg);
  void shutdown();

private:
  void logMessage(QString msg);
  void closeLogFile();
  void openLogFile();
  void writeLogHeader();

  void closeScanFile();
  void openScanFile();

public:  // Properties
  QxrdSettingsSaver m_Saver;

  Q_PROPERTY(int experimentKind READ get_ExperimentKind WRITE set_ExperimentKind)
  QCEP_INTEGER_PROPERTY(ExperimentKind)

  Q_PROPERTY(QString experimentDirectory     READ get_ExperimentDirectory WRITE set_ExperimentDirectory)
  QCEP_STRING_PROPERTY(ExperimentDirectory)

  Q_PROPERTY(QString experimentFileName     READ get_ExperimentFileName WRITE set_ExperimentFileName)
  QCEP_STRING_PROPERTY(ExperimentFileName)

  Q_PROPERTY(QString experimentName     READ get_ExperimentName WRITE set_ExperimentName)
  QCEP_STRING_PROPERTY(ExperimentName)

  Q_PROPERTY(QString experimentDescription     READ get_ExperimentDescription WRITE set_ExperimentDescription)
  QCEP_STRING_PROPERTY(ExperimentDescription)

  Q_PROPERTY(QString logFilePath     READ get_LogFilePath WRITE set_LogFilePath)
  QCEP_STRING_PROPERTY(LogFilePath)

  Q_PROPERTY(QString scanFilePath     READ get_ScanFilePath WRITE set_ScanFilePath)
  QCEP_STRING_PROPERTY(ScanFilePath)

  Q_PROPERTY(int    detectorType  READ get_DetectorType WRITE set_DetectorType)
  QCEP_INTEGER_PROPERTY(DetectorType)

  Q_PROPERTY(int    processorType READ get_ProcessorType WRITE set_ProcessorType)
  QCEP_INTEGER_PROPERTY(ProcessorType)

  Q_PROPERTY(int    defaultLayout   READ get_DefaultLayout WRITE set_DefaultLayout STORED false)
  QCEP_INTEGER_PROPERTY(DefaultLayout)

  Q_PROPERTY(int    runSpecServer    READ get_RunSpecServer WRITE set_RunSpecServer)
  QCEP_INTEGER_PROPERTY(RunSpecServer)

  Q_PROPERTY(int    specServerPort    READ get_SpecServerPort WRITE set_SpecServerPort)
  QCEP_INTEGER_PROPERTY(SpecServerPort)

  Q_PROPERTY(int    runSimpleServer    READ get_RunSimpleServer WRITE set_RunSimpleServer)
  QCEP_INTEGER_PROPERTY(RunSimpleServer)

  Q_PROPERTY(int    simpleServerPort    READ get_SimpleServerPort WRITE set_SimpleServerPort)
  QCEP_INTEGER_PROPERTY(SimpleServerPort)

private:
  QxrdApplication                *m_Application;
  QxrdWindow                     *m_Window;
  QSplashScreen                  *m_Splash;
  QxrdServerThread               *m_ServerThread;
  QxrdServer                     *m_Server;
  QxrdSimpleServerThread         *m_SimpleServerThread;
  QxrdSimpleServer               *m_SimpleServer;
  QxrdDataProcessorThread        *m_DataProcessorThread;
  QxrdDataProcessor              *m_DataProcessor;
  QxrdAcquisitionThread          *m_AcquisitionThread;
  QxrdAcquisition                *m_Acquisition;
  QxrdFileSaverThread            *m_FileSaverThread;

  FILE                           *m_LogFile;
  FILE                           *m_ScanFile;
};

#endif // QXRDEXPERIMENT_H
