#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qcepmacros.h"

#include <QApplication>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QSplashScreen>
#include "qcepproperty.h"
#include "qxrdresponsetimer.h"
#include "qxrdacquisitionthread.h"
#include "qxrddataprocessorthread.h"
#include "qxrdserverthread.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdallocatorthread.h"
#include "qxrdfilesaverthread.h"
#include "qxrdscriptenginethread.h"
#include "qxrdsettingssaverthread.h"

class QxrdWindow;
class QxrdNIDAQPluginInterface;

#ifdef HAVE_PERKIN_ELMER
class QxrdPerkinElmerPluginInterface;
#endif

class QxrdApplication : public QApplication
{
  Q_OBJECT;

public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();
  bool init(QSplashScreen *splash);

  QxrdAcquisitionThread *acquisitionThread();
  QxrdAcquisition *acquisition() const;
  QxrdWindow *window();
  QxrdDataProcessor *dataProcessor() const;
  QxrdAllocator *allocator() const;

#ifdef HAVE_PERKIN_ELMER
  QxrdPerkinElmerPluginInterface *perkinElmerPlugin();
#endif

  QxrdNIDAQPluginInterface *nidaqPlugin();

  void loadPlugins();

  void tiffWarning(const char* module, const char *msg);
  void tiffError(const char* module, const char *msg);

  static QxrdApplication* application();

  bool event(QEvent *ev);

public slots:
  void shutdownThreads();
  void possiblyQuit();
  void editPreferences();
  void debugChanged(int newValue);

  void readSettings();
  void writeSettings();

  void doSavePreferences();
  void doLoadPreferences();

  void splashMessage(const char *msg);
  void splashMessage(QString msg);

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

public:
  bool wantToQuit();
  FILE* logFile();

  void newLogFile(QString path);

  static QString hexArg(void *p);

private:
  void logMessage(QString msg);
  void closeLogFile();
  void openLogFile();
  void writeLogHeader();

  void setupTiffHandlers();
  void shutdownThread(QxrdThread *thread);

public:
  Q_PROPERTY(int    detectorType  READ get_DetectorType WRITE set_DetectorType);
  QCEP_INTEGER_PROPERTY(DetectorType);

  Q_PROPERTY(int    processorType READ get_ProcessorType WRITE set_ProcessorType);
  QCEP_INTEGER_PROPERTY(ProcessorType);

  Q_PROPERTY(int    debug         READ get_Debug WRITE set_Debug);
  QCEP_INTEGER_PROPERTY(Debug);

  Q_PROPERTY(int    runSpecServer    READ get_RunSpecServer WRITE set_RunSpecServer);
  QCEP_INTEGER_PROPERTY(RunSpecServer);

  Q_PROPERTY(int    specServerPort    READ get_SpecServerPort WRITE set_SpecServerPort);
  QCEP_INTEGER_PROPERTY(SpecServerPort);

  Q_PROPERTY(int    runSimpleServer    READ get_RunSimpleServer WRITE set_RunSimpleServer);
  QCEP_INTEGER_PROPERTY(RunSimpleServer);

  Q_PROPERTY(int    simpleServerPort    READ get_SimpleServerPort WRITE set_SimpleServerPort);
  QCEP_INTEGER_PROPERTY(SimpleServerPort);

  Q_PROPERTY(int    defaultLayout   READ get_DefaultLayout WRITE set_DefaultLayout STORED false);
  QCEP_INTEGER_PROPERTY(DefaultLayout);

  Q_PROPERTY(QString logFilePath     READ get_LogFilePath WRITE set_LogFilePath);
  QCEP_STRING_PROPERTY(LogFilePath);

  Q_PROPERTY(int    fileBrowserLimit   READ get_FileBrowserLimit WRITE set_FileBrowserLimit);
  QCEP_INTEGER_PROPERTY(FileBrowserLimit);

private:
  bool                            m_FreshStart;
  QSplashScreen                  *m_Splash;
  QxrdWindow                     *m_Window;
  QxrdServerThread               *m_ServerThread;
  QxrdServer                     *m_Server;
  QxrdSimpleServerThread         *m_SimpleServerThread;
  QxrdSimpleServer               *m_SimpleServer;
  QxrdDataProcessorThread        *m_DataProcessorThread;
  QxrdDataProcessor              *m_DataProcessor;
  QxrdAcquisitionThread          *m_AcquisitionThread;
  QxrdAcquisition                *m_Acquisition;
  QxrdAllocatorThread            *m_AllocatorThread;
  QxrdAllocator                  *m_Allocator;
  QxrdFileSaverThread            *m_FileSaverThread;
  QxrdScriptEngineThread         *m_ScriptEngineThread;
  QxrdScriptEngine               *m_ScriptEngine;
  QScriptEngineDebugger          *m_ScriptEngineDebugger;
  QxrdSettingsSaverThread        *m_SettingsSaverThread;
  QxrdSettingsSaver              *m_SettingsSaver;
  QxrdNIDAQPluginInterface       *m_NIDAQPluginInterface;
#ifdef HAVE_PERKIN_ELMER
  QxrdPerkinElmerPluginInterface *m_PerkinElmerPluginInterface;
#endif
  QxrdResponseTimer              *m_ResponseTimer;

  mutable QMutex                  m_LogFileMutex;
  FILE                           *m_LogFile;
};

#define HEXARG(a) arg(QxrdApplication::hexArg(a))

extern QxrdApplication *g_Application;

#endif
