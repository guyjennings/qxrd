#ifndef QXRDEXPERIMENT_H
#define QXRDEXPERIMENT_H

#define _CRT_SECURE_NO_WARNINGS

#include "qcepmacros.h"
#include "qcepproperty.h"

#include <QObject>
#include <QSharedPointer>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include "qcepexperiment.h"
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
#include "qxrddetectorthread.h"
#include "qxrddetectorthread-ptr.h"
#include "qxrddetector.h"
#include "qxrddetector-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdintegrator-ptr.h"

class QxrdExperiment : public QcepExperiment
{
  Q_OBJECT

public:
  QxrdExperiment(QString path, QxrdApplicationWPtr app);
  //  virtual bool init(QxrdExperimentThreadWPtr expthrd, QxrdExperimentWPtr exp, QSettings *settings);
  virtual ~QxrdExperiment();
  void initialize(QxrdExperimentThreadWPtr expthrd, QxrdExperimentWPtr exp, QSettings *settings);

  void openWindows();

  QxrdExperimentThreadWPtr experimentThread();

  QxrdAcquisitionThreadPtr acquisitionThread();
  QxrdAcquisitionWPtr acquisition() const;
  QxrdWindowPtr window();
  QxrdDataProcessorWPtr dataProcessor() const;
  QxrdCenterFinderWPtr centerFinder() const;
  QxrdIntegratorWPtr integrator() const;
  QxrdServerWPtr specServer();
  QxrdServerThreadWPtr specServerThread();
  QxrdSimpleServerWPtr simpleServer();
  QxrdSimpleServerThreadWPtr simpleServerThread();

  FILE* logFile();

  FILE* scanFile();
  void newScanFile(QString path);

  QxrdSettingsSaverPtr settingsSaver();
  QxrdScriptEngineWPtr scriptEngine();

public slots:
  void newLogFile(QString path);
  void openNewLogFile() const;

  void executeCommand(QString cmd);

  QString defaultExperimentDirectory(QString path) const;
  QString defaultExperimentFileName(QString path) const;
  QString defaultExperimentName(QString path) const;
  QString defaultDataDirectory(QString path) const;
  QString defaultLogName(QString path) const;
  QString defaultScanName(QString path) const;

  QString experimentFilePath() const;
  QString logFilePath() const;
  QString scanFilePath() const;

  void setExperimentFilePath(QString path);

  void saveExperimentCopyAs(QString path);
  void saveExperimentAs(QString path);

  void readSettings();
  void writeSettings();

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void logMessage(QString msg) const;
  void splashMessage(QString msg);
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void printLine(QString msg);

  void saveExperiment();

  void completeWork(int amt);
  void commenceWork(int amt);
  void finishedWork(int amt);
  void updateCompletionPercentage(int, int);

  void dump();
  QColor pickColor(QColor start=QColor(Qt::white));

  void evaluateScriptFiles(QStringList files);
  void evaluateScriptFile(QString path);

private slots:
  void onDetectorTypeChanged();

private:
  void closeLogFile() const;
  void openLogFile() const;
  void readInitialLogFile();

  void closeScanFile();

public:
  void openScanFile();

private:
  QxrdApplicationWPtr             m_Application;
  QxrdExperimentThreadWPtr        m_ExperimentThread;
  QxrdExperimentWPtr              m_Experiment;
  QxrdSettingsSaverPtr            m_SettingsSaver;

private:
  QxrdWindowSettingsPtr           m_WindowSettings;
  QxrdWindowPtr                   m_Window;
  QxrdServerThreadPtr             m_ServerThread;
  QxrdServerWPtr                  m_Server;
  QxrdSimpleServerThreadPtr       m_SimpleServerThread;
  QxrdSimpleServerWPtr            m_SimpleServer;
  QxrdDataProcessorThreadPtr      m_DataProcessorThread;
  QxrdDataProcessorWPtr           m_DataProcessor;
  QxrdAcquisitionThreadPtr        m_AcquisitionThread;
  QxrdAcquisitionWPtr             m_Acquisition;
  QxrdDetectorThreadPtr           m_DetectorThread;
  QxrdDetectorWPtr                m_Detector;
  QxrdFileSaverThreadPtr          m_FileSaverThread;
  QxrdFileSaverWPtr               m_FileSaver;
  QxrdScriptEngineThreadPtr       m_ScriptEngineThread;
  QxrdScriptEngineWPtr            m_ScriptEngine;
  QScriptEngineDebugger          *m_ScriptEngineDebugger;

  mutable QMutex                  m_LogFileMutex;
  mutable FILE                   *m_LogFile;

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

  Q_PROPERTY(QString dataDirectory     READ get_DataDirectory WRITE set_DataDirectory)
  QCEP_STRING_PROPERTY(DataDirectory)

  Q_PROPERTY(QString logFileName     READ get_LogFileName WRITE set_LogFileName)
  QCEP_STRING_PROPERTY(LogFileName)

  Q_PROPERTY(QString scanFileName     READ get_ScanFileName WRITE set_ScanFileName)
  QCEP_STRING_PROPERTY(ScanFileName)

  Q_PROPERTY(QString scanFileExtension     READ get_ScanFileExtension WRITE set_ScanFileExtension)
  QCEP_STRING_PROPERTY(ScanFileExtension)

  Q_PROPERTY(int     scanDataNegative      READ get_ScanDataNegative WRITE set_ScanDataNegative)
  QCEP_INTEGER_PROPERTY(ScanDataNegative)

  Q_PROPERTY(int    detectorType  READ get_DetectorType WRITE set_DetectorType)
  QCEP_INTEGER_PROPERTY(DetectorType)

  Q_PROPERTY(QString detectorTypeName READ get_DetectorTypeName WRITE set_DetectorTypeName STORED false)
  QCEP_STRING_PROPERTY(DetectorTypeName)

  Q_PROPERTY(int    detectorNumber  READ get_DetectorNumber WRITE set_DetectorNumber)
  QCEP_INTEGER_PROPERTY(DetectorNumber)

  Q_PROPERTY(int    processorType READ get_ProcessorType WRITE set_ProcessorType)
  QCEP_INTEGER_PROPERTY(ProcessorType)

  Q_PROPERTY(int    defaultLayout   READ get_DefaultLayout WRITE set_DefaultLayout STORED false)
  QCEP_INTEGER_PROPERTY(DefaultLayout)

  Q_PROPERTY(int    workCompleted READ get_WorkCompleted WRITE set_WorkCompleted STORED false)
  QCEP_INTEGER_PROPERTY(WorkCompleted)

  Q_PROPERTY(int    workTarget READ get_WorkTarget WRITE set_WorkTarget STORED false)
  QCEP_INTEGER_PROPERTY(WorkTarget)

  Q_PROPERTY(int    completionPercentage READ get_CompletionPercentage WRITE set_CompletionPercentage STORED false)
  QCEP_INTEGER_PROPERTY(CompletionPercentage)

  Q_PROPERTY(QString defaultScript READ get_DefaultScript WRITE set_DefaultScript)
  QCEP_STRING_PROPERTY(DefaultScript)

  Q_PROPERTY(QStringList extraScriptFiles READ get_ExtraScriptFiles WRITE set_ExtraScriptFiles)
  QCEP_STRING_LIST_PROPERTY(ExtraScriptFiles)
};

//typedef QSharedPointer<QxrdExperiment> QxrdExperimentPtr;
//typedef QWeakPointer<QxrdExperiment> QxrdExperimentWPtr;

#endif // QXRDEXPERIMENT_H
