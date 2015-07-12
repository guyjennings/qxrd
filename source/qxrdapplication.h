#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qcepmacros.h"
#include "qcepproperty.h"

#include "qcepapplication.h"
#include <QSettings>
#include "qxrdresponsetimer.h"
#include "qcepallocatorthread-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdexperiment.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdwelcomewindow-ptr.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qcepsettingssaver.h"
#include "qxrdsplashscreen-ptr.h"
#include "qcepdataobject-ptr.h"

#ifdef HAVE_PERKIN_ELMER
#include "qxrdperkinelmerplugininterface-ptr.h"
#endif

extern void doDeleteLater(QObject *object);

class QxrdApplication : public QcepApplication
{
  Q_OBJECT

public:
  QxrdApplication(int &argc, char **argv);
  virtual ~QxrdApplication();

  bool init(QxrdApplicationWPtr app, int &argc, char **argv);

  QcepAllocatorWPtr allocator() const;

#ifdef HAVE_PERKIN_ELMER
  QxrdPerkinElmerPluginInterfacePtr perkinElmerPlugin();
#endif

  QxrdNIDAQPluginInterfacePtr nidaqPlugin();

  void loadPlugins();

  void tiffWarning(const char* module, const char *msg);
  void tiffError(const char* module, const char *msg);

//  static QxrdApplicationPtr application();

  bool event(QEvent *ev);

  QStringList makeStringListFromArgs(int argc, char **argv);

  QWidget* window();

  void readDefaultSettings();
  void writeDefaultSettings();
  void appendRecentExperiment(QString path);

  QString newAnalysisExperiment(QString path);
  QString newPerkinElmerExperiment(QString path);
  QString newPilatusExperiment(QString path);

  QString normalizeExperimentName(QString filename);
  void setNewExperimentSettings(QSettings &settings, int type, QString filename);

public slots:
  void finish();

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
  void closeExperiment(QxrdExperimentPtr exp);

  void openedExperiment(QxrdExperimentThreadPtr expthrdw);
  void closedExperiment(QxrdExperimentThreadPtr expthrdw);
  QList<QxrdExperimentPtr> &experiments();

  QxrdExperimentPtr experiment(int i);

  void openWelcomeWindow();
  void closeWelcomeWindow();

  void doAboutQxrd();
  void doOpenQXRDWebPage();

  void possiblyQuit();
  void editGlobalPreferences();
  void debugChanged(qint64 newValue);

  void readSettings();
  void writeSettings();
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

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

  QString homePath();
  QString currentPath();
  QString rootPath();

  void lockerTimerElapsed();

  virtual void setDefaultObjectData(QcepDataObject *obj);

public:
  bool wantToQuit();

  void incLockerCount();

private slots:
  void hideSplash();

private:

  void setupTiffHandlers();

private:
  QcepObjectNamer      m_ObjectNamer;
  QcepSettingsSaverPtr m_Saver;

public:
  Q_PROPERTY(QStringList recentExperiments READ get_RecentExperiments WRITE set_RecentExperiments)
  QCEP_STRING_LIST_PROPERTY(RecentExperiments)

  Q_PROPERTY(int recentExperimentsSize READ get_RecentExperimentsSize WRITE set_RecentExperimentsSize)
  QCEP_INTEGER_PROPERTY(RecentExperimentsSize)

  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment)
  QCEP_STRING_PROPERTY(CurrentExperiment)

  Q_PROPERTY(QString currentDirectory READ get_CurrentDirectory WRITE set_CurrentDirectory)
  QCEP_STRING_PROPERTY(CurrentDirectory)

//  Q_PROPERTY(bool   openDirectly  READ get_OpenDirectly WRITE set_OpenDirectly)
//  QCEP_BOOLEAN_PROPERTY(OpenDirectly)

  Q_PROPERTY(qint64    debug         READ get_Debug WRITE set_Debug)
  QCEP_INTEGER64_PROPERTY(Debug)

  Q_PROPERTY(int    freshStart         READ get_FreshStart WRITE set_FreshStart STORED false)
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

  Q_PROPERTY(int lockerCount READ get_LockerCount WRITE set_LockerCount STORED false)
  QCEP_INTEGER_PROPERTY(LockerCount)

  Q_PROPERTY(double lockerRate READ get_LockerRate WRITE set_LockerRate STORED false)
  QCEP_DOUBLE_PROPERTY(LockerRate)

private:
  QxrdApplicationWPtr             m_Application;
  QList<QxrdExperimentThreadPtr>  m_ExperimentThreads;
  QList<QxrdExperimentPtr>        m_Experiments;

  QTimer                          m_SplashTimer;
  QxrdSplashScreenPtr             m_Splash;

  QxrdWelcomeWindowPtr            m_WelcomeWindow;
  QcepAllocatorThreadPtr          m_AllocatorThread;
  QcepAllocatorPtr                m_Allocator;
  QxrdNIDAQPluginInterfacePtr     m_NIDAQPluginInterface;
#ifdef HAVE_PERKIN_ELMER
  QxrdPerkinElmerPluginInterfacePtr m_PerkinElmerPluginInterface;
#endif
  QxrdResponseTimer              *m_ResponseTimer;

  QMutex                          m_SettingsMutex;

  QTimer                          m_LockerTimer;
  QTime                           m_LastLockerTime;
  int                             m_LastLockerCount;
};

#endif
