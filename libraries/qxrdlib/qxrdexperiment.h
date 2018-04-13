#ifndef QXRDEXPERIMENT_H
#define QXRDEXPERIMENT_H

#define _CRT_SECURE_NO_WARNINGS

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"

#include <QObject>
#include <QScriptEngine>
//#include <QScriptEngineDebugger>
#include <QTimer>
#include "qcepexperiment.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "qxrdserverthread-ptr.h"
#include "qxrdserver-ptr.h"
#include "qxrdsimpleserverthread-ptr.h"
#include "qxrdsimpleserver-ptr.h"
#include "qxrdfilesaverthread-ptr.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrdwindowsettings-ptr.h"
#include "qxrdscriptengine.h"
#include "qxrdscriptengine-ptr.h"
#include "qxrdscriptenginethread.h"
#include "qxrdscriptenginethread-ptr.h"
#include "qxrddetectorsettings.h"
#include "qxrddetectorsettings-ptr.h"
#include "qcepcenterfinder-ptr.h"
#include "qxrdintegrator-ptr.h"
#include "qcepcalibrantlibrary-ptr.h"
#include "qcepcalibrantlibrarymodel-ptr.h"
#include "qcepcalibrantdspacings-ptr.h"
#include "qcepcalibrantdspacingsmodel-ptr.h"
#include "qcepexperiment.h"
#include "qcepdataset-ptr.h"
#include "qcepdatasetmodel-ptr.h"
#include "qxrddetectorcontrolwindow-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdexperimentsettings-ptr.h"
#include "qxrdmainwindowsettings-ptr.h"
#include "qxrdjsengine-ptr.h"

class QXRD_EXPORT QxrdExperiment : public QcepExperiment
{
  Q_OBJECT

private:
  typedef QcepExperiment inherited;

public:
  Q_INVOKABLE QxrdExperiment(QString name);

  virtual ~QxrdExperiment();
  void initialize(QcepObjectWPtr parent);
  void initialize(QcepObjectWPtr parent,
                  QString        path,
                  int            mode);

  static QxrdExperimentWPtr findExperiment(QcepObjectWPtr p);

  static void registerMetaTypes();

  enum {
    DefaultMode,
    AcquisitionAllowed,
    ViewerOnly
  };

  void openWindows();
  void closeWindows();

  QxrdAppCommonWPtr application() const;
  QxrdExperimentThreadWPtr experimentThread() const;
  QxrdAcqCommonWPtr acquisition() const;
  QxrdProcessorWPtr processor() const;
  QcepCenterFinderWPtr centerFinder() const;
  QxrdIntegratorWPtr integrator() const;
  QxrdServerWPtr specServer();
  QxrdServerThreadWPtr specServerThread();
  QxrdSimpleServerWPtr simpleServer();
  QxrdSimpleServerThreadWPtr simpleServerThread();
  QcepCalibrantLibraryWPtr calibrantLibrary() const;
  QcepCalibrantLibraryModelWPtr calibrantLibraryModel() const;
  QcepCalibrantDSpacingsWPtr calibrantDSpacings() const;
  QcepCalibrantDSpacingsModelWPtr calibrantDSpacingsModel() const;

  QcepDatasetModelPtr dataset();
  QxrdFileSaverWPtr fileSaver() const;

  FILE* logFile();

  FILE* scanFile();
  void newScanFile(QString path);

  QxrdScriptEngineWPtr scriptEngine();
  QxrdJSEngineWPtr     jsEngine();

public slots:
  void newLogFile(QString path);
  void openNewLogFile();

  void executeCommand(QString cmd);
  QString systemCommand(QString cmd);

  void executeCommandJS(QString cmd);

  QString defaultDataDirectory(QString path) const;
  QString defaultLogName(QString path) const;
  QString defaultScanName(QString path) const;

  QString experimentFilePath() const;
  QString logFilePath() const;
  QString scanFilePath() const;

  QString backupFilePath(QString path, int n);
  bool    backupFileIfNeeded(FILE* f, int maxLenMB, QString path, int nBackups);

  void haltAutosaveTimer();

  void shutdownAndSave();

  void setExperimentFilePath(QString path);

  void saveExperimentAs(QString path);
  void saveExperimentCopyAs(QString path);

  void readExperimentSettings();
  void writeExperimentSettings();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void logMessage(QString msg);

  void saveExperiment();
  void saveExperimentAsText(QString filePath);

  void autoSaveExperiment();
  int  wasAutoSaved();

  void completeWork(int amt);
  void commenceWork(int amt);
  void finishedWork(int amt);
  void updateCompletionPercentage(int, int);

  void acquireStarted();
  void acquiredFrame(QString fileName,
                     int iphase,
                     int nphases,
                     int isum,
                     int nsum,
                     int igroup,
                     int ngroup);
  void acquireComplete();

  void setupAcquisition();

  void plotImage(QcepDoubleImageDataPtr img);

  QColor pickColor(QColor start=QColor(Qt::white));

  void evaluateScriptFiles(QStringList files);
  void evaluateScriptFile(QString path);

  void openWindow(QxrdMainWindowSettingsWPtr set);

  void defaultWindowSettings();

  void openWatcher(QString patt);

private:

  void closeLogFile();
  void openLogFile();
  void readInitialLogFile();

  void closeScanFile();

public:
  void openScanFile();

private:
  QxrdAppCommonWPtr               m_Application;
  QxrdExperimentThreadWPtr        m_ExperimentThread;
  QxrdServerThreadPtr             m_ServerThread;
  QxrdServerWPtr                  m_Server;
  QxrdSimpleServerThreadPtr       m_SimpleServerThread;
  QxrdSimpleServerWPtr            m_SimpleServer;
  QxrdProcessorPtr                m_Processor;
  QcepCalibrantLibraryPtr         m_CalibrantLibrary;
  QcepCalibrantLibraryModelPtr    m_CalibrantLibraryModel;
  QcepCalibrantDSpacingsPtr       m_CalibrantDSpacings;
  QcepCalibrantDSpacingsModelPtr  m_CalibrantDSpacingsModel;
  QxrdAcqCommonPtr                m_Acquisition;
  QxrdFileSaverThreadPtr          m_FileSaverThread;
  QxrdFileSaverWPtr               m_FileSaver;
  QxrdScriptEnginePtr             m_ScriptEngine;

  QxrdJSEnginePtr                 m_ScriptEngineJS;

  QcepDatasetPtr                  m_Dataset;
  QcepDatasetModelPtr             m_DatasetModel;
  mutable QMutex                  m_LogFileMutex;
  mutable FILE                   *m_LogFile;

  QMutex                          m_ScanFileMutex;
  FILE                           *m_ScanFile;

  QMutex                          m_ExperimentFileMutex;

  QTimer                          m_AutoSaveTimer;
  int                             m_WasAutoSaved;

public:  // Properties
  Q_PROPERTY(int experimentMode READ get_ExperimentMode WRITE set_ExperimentMode STORED false)
  QCEP_INTEGER_PROPERTY(ExperimentMode)

  Q_PROPERTY(QString qxrdVersion       READ get_QxrdVersion   WRITE set_QxrdVersion)
  QCEP_STRING_PROPERTY(QxrdVersion)

  Q_PROPERTY(QString dataDirectory     READ get_DataDirectory WRITE set_DataDirectory)
  QCEP_STRING_PROPERTY(DataDirectory)

  Q_PROPERTY(QString logFileName     READ get_LogFileName WRITE set_LogFileName)
  QCEP_STRING_PROPERTY(LogFileName)

  Q_PROPERTY(int logMaxLengthMB READ get_LogMaxLengthMB WRITE set_LogMaxLengthMB)
  QCEP_INTEGER_PROPERTY(LogMaxLengthMB)

  Q_PROPERTY(int logMaxSaved READ get_LogMaxSaved WRITE set_LogMaxSaved)
  QCEP_INTEGER_PROPERTY(LogMaxSaved)

  Q_PROPERTY(QString scanFileName     READ get_ScanFileName WRITE set_ScanFileName)
  QCEP_STRING_PROPERTY(ScanFileName)

  Q_PROPERTY(QString scanFileExtension     READ get_ScanFileExtension WRITE set_ScanFileExtension)
  QCEP_STRING_PROPERTY(ScanFileExtension)

  Q_PROPERTY(int     scanDataNegative      READ get_ScanDataNegative WRITE set_ScanDataNegative)
  QCEP_INTEGER_PROPERTY(ScanDataNegative)

  Q_PROPERTY(int  scanMaxLengthMB READ get_ScanMaxLengthMB WRITE set_ScanMaxLengthMB)
  QCEP_INTEGER_PROPERTY(ScanMaxLengthMB)

  Q_PROPERTY(int scanMaxSaved READ get_ScanMaxSaved WRITE set_ScanMaxSaved)
  QCEP_INTEGER_PROPERTY(ScanMaxSaved)

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

  Q_PROPERTY(int isReading READ get_IsReading WRITE set_IsReading STORED false)
  QCEP_INTEGER_PROPERTY(IsReading)
};

Q_DECLARE_METATYPE(QxrdExperiment*)

#endif // QXRDEXPERIMENT_H
