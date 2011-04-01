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

//class QxrdAcquisitionThread;
//class QxrdAcquisition;
class QxrdWindow;
//class QxrdDataProcessorThread;
//class QxrdDataProcessor;
class QxrdNIDAQPluginInterface;
//class QxrdServerThread;
//class QxrdServer;
//class QxrdSimpleServerThread;
//class QxrdSimpleServer;
//class QxrdAllocatorThread;
//class QxrdAllocator;
//class QxrdFileSaverThread;
//class QxrdScriptEngineThread;
//class QxrdScriptEngine;
//class QxrdSettingsSaverThread;
//class QxrdSettingsSaver;

#ifdef HAVE_PERKIN_ELMER
class QxrdPerkinElmerPluginInterface;
#endif

class QxrdApplication : public QApplication
{
  Q_OBJECT;

public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();
  void init(QSplashScreen *splash);

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

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

public:
  bool wantToQuit();

private:
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
};

extern QxrdApplication *g_Application;

#endif
