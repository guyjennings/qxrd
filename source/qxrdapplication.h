#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qcepmacros.h"

#include <QApplication>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QSplashScreen>

#include "qxrdforwardtypes.h"
#include "qxrddataprocessor.h"
#include "qxrdsettingssaverthread.h"

#ifdef HAVE_PERKIN_ELMER
#include "qxrdperkinelmerplugininterface.h"
#endif

#include "qxrdnidaqplugininterface.h"

extern QxrdApplication *g_Application;

class QxrdApplication : public QApplication
{
  Q_OBJECT;

public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();
  void init(QSplashScreen *splash);

  QxrdAcquisitionThreadPtr acquisitionThread();
  QxrdAcquisitionPtr acquisition() const;
  QxrdWindowPtr window();
  QxrdDataProcessorPtr dataProcessor() const;

#ifdef HAVE_PERKIN_ELMER
  QxrdPerkinElmerPluginInterface *perkinElmerPlugin();
#endif

  QxrdNIDAQPluginInterface *nidaqPlugin();

  void loadPlugins();

  void tiffWarning(const char* module, const char *msg);
  void tiffError(const char* module, const char *msg);

  static QxrdApplication* application();

public slots:
  void shutdownThreads();
  void possiblyQuit();
  void editPreferences();
  void debugChanged(int newValue);

  void readSettings();
  void writeSettings();
  void splashMessage(const char *msg);

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

public:
  bool wantToQuit();

private:
  void setupTiffHandlers();

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

private:
  QSplashScreen                  *m_Splash;
  QxrdWindowPtr                   m_Window;
  QxrdServerThreadPtr             m_ServerThread;
  QxrdServerPtr                   m_Server;
  QxrdSimpleServerThreadPtr       m_SimpleServerThread;
  QxrdSimpleServerPtr             m_SimpleServer;
  QxrdDataProcessorThreadPtr      m_DataProcessorThread;
  QxrdDataProcessorPtr            m_DataProcessor;
  QxrdAcquisitionThreadPtr        m_AcquisitionThread;
  QxrdAcquisitionPtr              m_Acquisition;
  QxrdAllocatorThreadPtr          m_AllocatorThread;
  QxrdAllocatorPtr                m_Allocator;
  QxrdFileSaverThreadPtr          m_FileSaverThread;
  QxrdScriptEngineThreadPtr       m_ScriptEngineThread;
  QxrdScriptEnginePtr             m_ScriptEngine;
  QScriptEngineDebugger          *m_ScriptEngineDebugger;
  QxrdSettingsSaverThreadPtr      m_SettingsSaverThread;
  QxrdSettingsSaverPtr            m_SettingsSaver;
  QxrdNIDAQPluginInterface       *m_NIDAQPluginInterface;
#ifdef HAVE_PERKIN_ELMER
  QxrdPerkinElmerPluginInterface *m_PerkinElmerPluginInterface;
#endif
};

#endif
