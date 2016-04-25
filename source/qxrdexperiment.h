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
#include "qcepsettingssaver.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorthread-ptr.h"
#include "qxrddetector.h"
#include "qxrddetector-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdintegrator-ptr.h"
#include "qxrdcalibrantlibrary-ptr.h"
#include "qxrdcalibrantlibrarymodel-ptr.h"
#include "qxrdcalibrantdspacings-ptr.h"
#include "qxrdcalibrantdspacingsmodel-ptr.h"
#include "qcepexperiment.h"
#include "qcepdataset-ptr.h"
#include "qcepdatasetmodel-ptr.h"
#include "qxrddetectorcontrolwindow-ptr.h"

class QxrdExperiment : public QcepExperiment
{
  Q_OBJECT

public:
  QxrdExperiment(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplicationWPtr app);
  virtual ~QxrdExperiment();
  virtual void initialize(QSettings *settings);

  void openWindows();
  void closeWindows();

  QxrdExperimentThreadWPtr experimentThread();

//  QxrdAcquisitionThreadPtr acquisitionThread();
  QxrdAcquisitionWPtr acquisition() const;
  QxrdWindowPtr window();
  QxrdDataProcessorWPtr dataProcessor() const;
  QxrdCenterFinderWPtr centerFinder() const;
  QxrdIntegratorWPtr integrator() const;
  QxrdServerWPtr specServer();
  QxrdServerThreadWPtr specServerThread();
  QxrdSimpleServerWPtr simpleServer();
  QxrdSimpleServerThreadWPtr simpleServerThread();
  QxrdCalibrantLibraryWPtr calibrantLibrary() const;
  QxrdCalibrantLibraryModelWPtr calibrantLibraryModel() const;
  QxrdCalibrantDSpacingsWPtr calibrantDSpacings() const;
  QxrdCalibrantDSpacingsModelWPtr calibrantDSpacingsModel() const;

  QcepDatasetModelPtr dataset();
  QxrdFileSaverWPtr fileSaver() const;

  FILE* logFile();

  FILE* scanFile();
  void newScanFile(QString path);

//  QcepSettingsSaverPtr settingsSaver();
  QxrdScriptEngineWPtr scriptEngine();

public slots:
  void newLogFile(QString path);
  void openNewLogFile() const;

  void executeCommand(QString cmd);
  QString systemCommand(QString cmd);

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

//  void openAcquisitionWindow();

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

private:
  QMutex                          m_Mutex;
  QxrdWindowSettingsPtr           m_WindowSettings;
  QxrdWindowPtr                   m_Window;
//  QxrdDetectorControlWindowPtr        m_AcquisitionWindow;
  QxrdServerThreadPtr             m_ServerThread;
  QxrdServerWPtr                  m_Server;
  QxrdSimpleServerThreadPtr       m_SimpleServerThread;
  QxrdSimpleServerWPtr            m_SimpleServer;
  QxrdDataProcessorPtr            m_DataProcessor;
  QxrdCalibrantLibraryPtr         m_CalibrantLibrary;
  QxrdCalibrantLibraryModelPtr    m_CalibrantLibraryModel;
  QxrdCalibrantDSpacingsPtr       m_CalibrantDSpacings;
  QxrdCalibrantDSpacingsModelPtr  m_CalibrantDSpacingsModel;
  QxrdAcquisitionPtr              m_Acquisition;
//  QxrdDetectorThreadPtr           m_DetectorThread;
//  QxrdDetectorWPtr                m_Detector;
  QxrdFileSaverThreadPtr          m_FileSaverThread;
  QxrdFileSaverWPtr               m_FileSaver;
//  QxrdScriptEngineThreadPtr       m_ScriptEngineThread;
//  QxrdScriptEngineWPtr            m_ScriptEngine;
  QxrdScriptEnginePtr             m_ScriptEngine;
  QScriptEngineDebugger          *m_ScriptEngineDebugger;

  QcepDatasetPtr                  m_Dataset;
  QcepDatasetModelPtr             m_DatasetModel;
  mutable QMutex                  m_LogFileMutex;
  mutable FILE                   *m_LogFile;

  QMutex                          m_ScanFileMutex;
  FILE                           *m_ScanFile;

  QMutex                          m_ExperimentFileMutex;

public:  // Properties
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

  Q_PROPERTY(int fontSize READ get_FontSize WRITE set_FontSize)
  QCEP_INTEGER_PROPERTY(FontSize)

  Q_PROPERTY(int spacing READ get_Spacing WRITE set_Spacing)
  QCEP_INTEGER_PROPERTY(Spacing)
};

#endif // QXRDEXPERIMENT_H
