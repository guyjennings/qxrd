#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qcepmacros.h"
#include "qcepproperty.h"

#include <QApplication>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QSettings>
#include "qxrdresponsetimer.h"
#include "qxrdallocatorthread.h"
#include "qxrdscriptenginethread.h"
//#include "qxrddefaultapplication.h"
#include "qxrdexperiment.h"
#include "qxrdexperimentthread.h"
#include "qxrdwelcomewindow.h"
#include "qxrdsplashscreen.h"

#ifdef HAVE_PERKIN_ELMER
class QxrdPerkinElmerPluginInterface;
#endif

class QxrdApplication : public QApplication
{
  Q_OBJECT

public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();
  bool init(QSplashScreen *splash);

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

  QStringList makeStringList(int argc, char **argv);

  QWidget* window();
  QxrdScriptEngine* scriptEngine();

  void readDefaultSettings();
  void writeDefaultSettings();
  void appendRecentExperiment(QString path);

  QString newAnalysisExperiment(QString path);
  QString newPerkinElmerExperiment(QString path);
  QString newPilatusExperiment(QString path);

  QString normalizeExperimentName(QString filename);
  void setNewExperimentSettings(QSettings &settings, int type, QString filename);

public slots:
  void doNewPerkinElmerAcquisition();
  void doNewPilatusAcquisition();
  void doNewSimulatedAcquisition();
  void doNewPerkinElmerAnalysis();
  void doNewPilatusAnalysis();
  void doNewGenericAnalysis();

  void chooseNewExperiment();
  void chooseExistingExperiment();
  void openRecentExperiment(QString path);

  void activateExperiment(QString path);
  void openExperiment(QString path);

  void openedExperiment(QxrdExperimentThread *expthrd);
  void closedExperiment(QObject *obj);
  QList<QxrdExperiment*> &experiments();

  QxrdExperiment* experiment(int i);

  void openWelcomeWindow();
  void closeWelcomeWindow();

  void doAboutQxrd();
  void doOpenQXRDWebPage();

  void shutdownThreads();
  void possiblyQuit();
  void editGlobalPreferences();
  void debugChanged(int newValue);

  void readSettings();
  void writeSettings();
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void doSavePreferences();
  void doLoadPreferences();

  void loadPreferences(QString path);
  void savePreferences(QString path);

  void splashMessage(QString msg);
  void logMessage(QString msg);
  void warningMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

  void processEventCounter();

  void executeCommand(QString cmd);

  QString homePath();
  QString currentPath();
  QString rootPath();

public:
  bool wantToQuit();

  static QString hexArg(void *p);

private:

  void setupTiffHandlers();
  void shutdownThread(QxrdThread *thread);

public:
  QxrdSettingsSaver m_Saver;

public:
  Q_PROPERTY(QStringList recentExperiments READ get_RecentExperiments WRITE set_RecentExperiments)
  QCEP_STRING_LIST_PROPERTY(RecentExperiments)

  Q_PROPERTY(int recentExperimentsSize READ get_RecentExperimentsSize WRITE set_RecentExperimentsSize)
  QCEP_INTEGER_PROPERTY(RecentExperimentsSize)

  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment)
  QCEP_STRING_PROPERTY(CurrentExperiment)

  Q_PROPERTY(QString currentDirectory READ get_CurrentDirectory WRITE set_CurrentDirectory)
  QCEP_STRING_PROPERTY(CurrentDirectory)

  Q_PROPERTY(bool   openDirectly  READ get_OpenDirectly WRITE set_OpenDirectly)
  QCEP_BOOLEAN_PROPERTY(OpenDirectly)

  Q_PROPERTY(int    debug         READ get_Debug WRITE set_Debug)
  QCEP_INTEGER_PROPERTY(Debug)

  Q_PROPERTY(int    freshStart         READ get_FreshStart WRITE set_FreshStart)
  QCEP_INTEGER_PROPERTY(FreshStart)

  Q_PROPERTY(int    fileBrowserLimit   READ get_FileBrowserLimit WRITE set_FileBrowserLimit)
  QCEP_INTEGER_PROPERTY(FileBrowserLimit)

  Q_PROPERTY(int    messageWindowLines   READ get_MessageWindowLines WRITE set_MessageWindowLines)
  QCEP_INTEGER_PROPERTY(MessageWindowLines)

  Q_PROPERTY(int    updateIntervalMsec   READ get_UpdateIntervalMsec WRITE set_UpdateIntervalMsec)
  QCEP_INTEGER_PROPERTY(UpdateIntervalMsec)

  Q_PROPERTY(int    argc       READ get_Argc      WRITE set_Argc STORED false)
  QCEP_INTEGER_PROPERTY(Argc)

  Q_PROPERTY(QStringList argv READ get_Argv WRITE set_Argv STORED false)
  QCEP_STRING_LIST_PROPERTY(Argv)

  Q_PROPERTY(int    guiWanted  READ get_GuiWanted WRITE set_GuiWanted STORED false)
  QCEP_INTEGER_PROPERTY(GuiWanted)

  Q_PROPERTY(QStringList cmdList READ get_CmdList WRITE set_CmdList STORED false)
  QCEP_STRING_LIST_PROPERTY(CmdList)

  Q_PROPERTY(QStringList fileList READ get_FileList WRITE set_FileList STORED false)
  QCEP_STRING_LIST_PROPERTY(FileList)

private:
  QList<QxrdExperimentThreadPtr>  m_ExperimentThreads;
  QList<QxrdExperiment*>          m_Experiments;

  QxrdWelcomeWindow              *m_WelcomeWindow;
  QxrdAllocatorThread            *m_AllocatorThread;
  QxrdAllocator                  *m_Allocator;
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

#define HEXARG(a) arg(QxrdApplication::hexArg(a))

extern QxrdApplication *g_Application;

#endif
