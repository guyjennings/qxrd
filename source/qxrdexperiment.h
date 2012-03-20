#ifndef QXRDEXPERIMENT_H
#define QXRDEXPERIMENT_H

#define _CRT_SECURE_NO_WARNINGS

#include "qcepmacros.h"
#include "qcepproperty.h"

#include <QObject>
#include <QSharedPointer>
#include <QSplashScreen>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include "qxrdapplication-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisitionthread-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddataprocessorthread-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdserverthread-ptr.h"
#include "qxrdserver-ptr.h"
#include "qxrdsimpleserverthread-ptr.h"
#include "qxrdsimpleserver-ptr.h"
#include "qxrdfilesaverthread-ptr.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrdwindowsettings.h"
#include "qxrdscriptengine.h"
#include "qxrdscriptengine-ptr.h"
#include "qxrdscriptenginethread.h"
#include "qxrdscriptenginethread-ptr.h"
#include "qxrdsettingssaver.h"

class QxrdExperiment : public QObject
{
  Q_OBJECT

public:
  QxrdExperiment(QString path, QxrdApplication *app, QSettings *settings);
  virtual bool init(QxrdExperimentThreadWPtr expthrd, QxrdExperimentWPtr exp, QSettings *settings);
  virtual ~QxrdExperiment();

  QxrdExperimentThreadWPtr experimentThread();
  QxrdAcquisitionThreadPtr acquisitionThread();
  QxrdAcquisitionPtr acquisition() const;
  QxrdWindow *window();
  QxrdDataProcessorWPtr dataProcessor() const;

  FILE* logFile();
  void newLogFile(QString path);

  FILE* scanFile();
  void newScanFile(QString path);

  QxrdSettingsSaverPtr settingsSaver();
public slots:
  QxrdScriptEnginePtr scriptEngine();

  void executeCommand(QString cmd);

  QString defaultExperimentDirectory(QString path);
  QString defaultExperimentFileName(QString path);
  QString defaultExperimentName(QString path);
  QString defaultLogName(QString path);
  QString defaultScanName(QString path);

  QString experimentFilePath();
  QString logFilePath();
  QString scanFilePath();

  void setExperimentFilePath(QString path);

  void saveExperimentCopyAs(QString path);
  void saveExperimentAs(QString path);

  void readSettings();
  void writeSettings();
  virtual void readSettings(QSettings *settings, QString section="experiment");
  virtual void writeSettings(QSettings *settings, QString section="experiment");

  void logMessage(QString msg);
  void splashMessage(QString msg);
  void criticalMessage(QString msg);
  void statusMessage(QString msg);
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

  void saveExperiment();

  void completeWork(int amt);
  void commenceWork(int amt);
  void finishedWork(int amt);
  void updateCompletionPercentage(int, int);


private:
  void closeLogFile();
  void openLogFile();

  void closeScanFile();
  void openScanFile();

private:
  QxrdApplication             *m_Application;
  QxrdExperimentThreadWPtr     m_ExperimentThread;
  QxrdSettingsSaverPtr         m_SettingsSaver;

private:
  QxrdWindowSettingsPtr           m_WindowSettings;
  QxrdWindow                     *m_Window;
  QSplashScreen                  *m_Splash;
  QxrdServerThreadPtr             m_ServerThread;
  QxrdServerPtr                   m_Server;
  QxrdSimpleServerThreadPtr       m_SimpleServerThread;
  QxrdSimpleServerPtr             m_SimpleServer;
  QxrdDataProcessorThreadPtr      m_DataProcessorThread;
  QxrdDataProcessorPtr            m_DataProcessor;
  QxrdAcquisitionThreadPtr        m_AcquisitionThread;
  QxrdAcquisitionPtr              m_Acquisition;
  QxrdFileSaverThreadPtr          m_FileSaverThread;
  QxrdFileSaverPtr                m_FileSaver;
  QxrdScriptEngineThreadPtr       m_ScriptEngineThread;
  QxrdScriptEnginePtr             m_ScriptEngine;
  QScriptEngineDebugger          *m_ScriptEngineDebugger;

  QMutex                          m_LogFileMutex;
  FILE                           *m_LogFile;

  QMutex                          m_ScanFileMutex;
  FILE                           *m_ScanFile;

  QMutex                          m_ExperimentFileMutex;

public:  // Properties
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

  Q_PROPERTY(QString logFileName     READ get_LogFileName WRITE set_LogFileName)
  QCEP_STRING_PROPERTY(LogFileName)

  Q_PROPERTY(QString scanFileName     READ get_ScanFileName WRITE set_ScanFileName)
  QCEP_STRING_PROPERTY(ScanFileName)

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

  Q_PROPERTY(int    workCompleted READ get_WorkCompleted WRITE set_WorkCompleted STORED false)
  QCEP_INTEGER_PROPERTY(WorkCompleted)

  Q_PROPERTY(int    workTarget READ get_WorkTarget WRITE set_WorkTarget STORED false)
  QCEP_INTEGER_PROPERTY(WorkTarget)

  Q_PROPERTY(int    completionPercentage READ get_CompletionPercentage WRITE set_CompletionPercentage STORED false)
  QCEP_INTEGER_PROPERTY(CompletionPercentage)
};

typedef QSharedPointer<QxrdExperiment> QxrdExperimentPtr;
typedef QWeakPointer<QxrdExperiment> QxrdExperimentWPtr;

#endif // QXRDEXPERIMENT_H
