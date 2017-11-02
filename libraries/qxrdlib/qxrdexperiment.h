#ifndef QXRDEXPERIMENT_H
#define QXRDEXPERIMENT_H

#define _CRT_SECURE_NO_WARNINGS

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"

#include <QObject>
#include <QSharedPointer>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QTimer>
#include "qcepexperiment.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperiment-ptr.h"
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
#include "qxrdwindowsettings-ptr.h"
#include "qxrdscriptengine.h"
#include "qxrdscriptengine-ptr.h"
#include "qxrdscriptenginethread.h"
#include "qxrdscriptenginethread-ptr.h"
#include "qxrddetectorsettings.h"
#include "qxrddetectorsettings-ptr.h"
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
#include "qxrdexperimentthread-ptr.h"
#include "qxrdexperimentsettings-ptr.h"
#include "qxrdjsengine-ptr.h"

class QXRD_EXPORT QxrdExperiment : public QcepExperiment
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdExperiment(QString name);

  static QxrdExperimentPtr newExperiment(QString path,
                                         QxrdApplicationWPtr app,
                                         QxrdExperimentSettingsPtr set);

  virtual ~QxrdExperiment();
  void initialize(QxrdExperimentSettingsPtr settings);

  virtual void addChildPtr(QcepObjectPtr child);
  virtual void removeChildPtr(QcepObjectPtr child);

  static void registerMetaTypes();

  void openWindows();
  void closeWindows();

  void setExperimentApplication(QxrdApplicationWPtr app);

  QxrdApplicationWPtr application() const;
  QxrdExperimentThreadPtr experimentThread() const;
  QxrdExperimentWPtr experiment();
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
  QxrdJSEngineWPtr     jsEngine();

public slots:
  void newLogFile(QString path);
  void openNewLogFile() const;

  void executeCommand(QString cmd);
  QString systemCommand(QString cmd);

  void executeCommandJS(QString cmd);

  QString defaultDataDirectory(QString path) const;
  QString defaultLogName(QString path) const;
  QString defaultScanName(QString path) const;

  QString experimentFilePath() const;
  QString logFilePath() const;
  QString scanFilePath() const;

  void setExperimentFilePath(QString path);

  void saveExperimentAs(QString path);
  void saveExperimentCopyAs(QString path);

  void saveExperiment2(QString path);

  void readSettings();
  void writeSettings();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void logMessage(QString msg) const;
  void splashMessage(QString msg);
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void printLine(QString msg) const;

  void saveExperiment();
  void saveExperimentAsText(QString filePath);

  void readObjectTreeFromText(QString filePath);

  void completeWork(int amt);
  void commenceWork(int amt);
  void finishedWork(int amt);
  void updateCompletionPercentage(int, int);

  void plotImage(QcepDoubleImageDataPtr img);

  void dump();
  QColor pickColor(QColor start=QColor(Qt::white));

  void evaluateScriptFiles(QStringList files);
  void evaluateScriptFile(QString path);

//  void openAcquisitionWindow();

  void onAutoSaveTimer();

  void defaultWindowSettings();

private:

  void closeLogFile() const;
  void openLogFile() const;
  void readInitialLogFile();

  void closeScanFile();

  void displayPushedMessages() const;
  void pushMessage(QString msg) const;

public:
  void openScanFile();

private:
  QxrdApplicationWPtr             m_Application;

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

  QxrdJSEnginePtr                 m_ScriptEngineJS;

  QcepDatasetPtr                  m_Dataset;
  QcepDatasetModelPtr             m_DatasetModel;
  mutable QMutex                  m_LogFileMutex;
  mutable FILE                   *m_LogFile;

  QMutex                          m_ScanFileMutex;
  FILE                           *m_ScanFile;

  QMutex                          m_ExperimentFileMutex;

  QTimer                          m_AutoSaveTimer;

  mutable QStringList             m_PushedMessages;

public:  // Properties
  Q_PROPERTY(QString qxrdVersion       READ get_QxrdVersion   WRITE set_QxrdVersion)
  QCEP_STRING_PROPERTY(QxrdVersion)

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

  Q_PROPERTY(int isReading READ get_IsReading WRITE set_IsReading STORED false)
  QCEP_INTEGER_PROPERTY(IsReading)
};

Q_DECLARE_METATYPE(QxrdExperiment*)

#endif // QXRDEXPERIMENT_H
