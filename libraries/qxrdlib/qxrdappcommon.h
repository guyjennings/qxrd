#ifndef QXRDAPPCOMMON_H
#define QXRDAPPCOMMON_H

#include "qxrdlib_global.h"
#include "qcepproperty.h"
#include "qcepapplication.h"
#include "qxrdwelcomewindow-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdmainwindowsettings-ptr.h"
#include "qxrdappcommon-ptr.h"
#include <QTimer>
#include "qcepallocator-ptr.h"
#include "qxrddebug.h"
#include "qxrdstartupwindow-ptr.h"

class QXRD_EXPORT QxrdAppCommon : public QcepApplication
{
  Q_OBJECT

private:
  typedef QcepApplication inherited;

public:
  explicit QxrdAppCommon(int &argc,
                         char **argv);
  virtual ~QxrdAppCommon();
  void initializeRoot();

  static QxrdAppCommonWPtr findApplication(QcepObjectWPtr p);

  virtual void finish();

  void appendCommand(QString cmd);
  void appendScript(QString script);
  void appendFile(QString file);
  void appendWatcher(QString patt);
  void appendPlugin(QString dir);

  void parseCommandLine(bool wantFullOptions);

  Q_INVOKABLE void splashMessage(QString msg);

  void tiffWarning(const char* module, const char *msg);
  void tiffError(const char* module, const char *msg);

  void openFile(QString filePath);
  void openWatcher(QString pattern);

  virtual void openStartupWindow() = 0;
  virtual void closeStartupWindow() = 0;

  void openWelcomeWindow();
  void closeWelcomeWindow();

  virtual QString applicationDescription() = 0;

  virtual void openExperiment(QString path) = 0;
  virtual void openRecentExperiment(QString path);
  void appendRecentExperiment(QString path);
  void closeExperiment(QxrdExperimentWPtr expw);

  Q_INVOKABLE virtual void openWindow(QxrdMainWindowSettingsWPtr set);

  virtual void readApplicationSettings() = 0;
  virtual void writeApplicationSettings() = 0;

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  virtual void editGlobalPreferences() = 0;
  virtual void createNewExperiment() = 0;
  virtual void chooseExistingExperiment() = 0;

  virtual void doAboutQxrd();
  virtual void doOpenQXRDWebPage();
  virtual void doOpenURL(QString url);

  QxrdExperimentPtr getFirstExperiment();

  void openedExperiment(QxrdExperimentThreadWPtr expwthr);
  void closedExperiment(QxrdExperimentThreadWPtr expwthr);
  QList<QxrdExperimentWPtr> &experiments();

  QxrdExperimentPtr experiment(int i) const;

  QcepAllocatorWPtr allocator() const;

  void possiblyQuit();
  bool wantToQuit();

  Q_ENUM(QcepDebugFlags)
  Q_ENUM(QxrdDebugFlags)

  void debugChanged(qint64 newValue);

  Q_INVOKABLE int  debugFlag(QString f);
  Q_INVOKABLE QString debugFlagName(int i);
  Q_INVOKABLE QString debugFlagOption(int i);
  Q_INVOKABLE QString debugFlagDescription(int i);
  Q_INVOKABLE int  debugFlagCount();
  Q_INVOKABLE void listDebugFlags();

private:
  QStringList makeStringListFromArgs(int argc, char **argv);

public:
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

  Q_PROPERTY(QStringList watcherList READ get_WatcherList WRITE set_WatcherList STORED false)
  QCEP_STRING_LIST_PROPERTY(WatcherList)

  Q_PROPERTY(QStringList pluginList READ get_PluginList WRITE set_PluginList STORED false)
  QCEP_STRING_LIST_PROPERTY(PluginList)

  Q_PROPERTY(qint64    debug         READ get_Debug WRITE set_Debug)
  QCEP_INTEGER64_PROPERTY(Debug)

  Q_PROPERTY(int    openNew         READ get_OpenNew WRITE set_OpenNew STORED false)
  QCEP_INTEGER_PROPERTY(OpenNew)

  Q_PROPERTY(int    freshStart         READ get_FreshStart WRITE set_FreshStart STORED false)
  QCEP_INTEGER_PROPERTY(FreshStart)

  Q_PROPERTY(int    startDetectors READ get_StartDetectors WRITE set_StartDetectors STORED false)
  QCEP_INTEGER_PROPERTY(StartDetectors)

  Q_PROPERTY(QString currentExperiment READ get_CurrentExperiment WRITE set_CurrentExperiment)
  QCEP_STRING_PROPERTY(CurrentExperiment)

  Q_PROPERTY(QStringList recentExperiments READ get_RecentExperiments WRITE set_RecentExperiments)
  QCEP_STRING_LIST_PROPERTY(RecentExperiments)

  Q_PROPERTY(int recentExperimentsSize READ get_RecentExperimentsSize WRITE set_RecentExperimentsSize)
  QCEP_INTEGER_PROPERTY(RecentExperimentsSize)

  Q_PROPERTY(int experimentCount READ get_ExperimentCount WRITE set_ExperimentCount STORED false)
  QCEP_INTEGER_PROPERTY(ExperimentCount)

  Q_PROPERTY(QString currentDirectory READ get_CurrentDirectory WRITE set_CurrentDirectory)
  QCEP_STRING_PROPERTY(CurrentDirectory)

  Q_PROPERTY(int    fileBrowserLimit   READ get_FileBrowserLimit WRITE set_FileBrowserLimit)
  QCEP_INTEGER_PROPERTY(FileBrowserLimit)

  Q_PROPERTY(int    messageWindowLines   READ get_MessageWindowLines WRITE set_MessageWindowLines)
  QCEP_INTEGER_PROPERTY(MessageWindowLines)

  Q_PROPERTY(int    updateIntervalMsec   READ get_UpdateIntervalMsec WRITE set_UpdateIntervalMsec)
  QCEP_INTEGER_PROPERTY(UpdateIntervalMsec)

  Q_PROPERTY(int lockerCount READ get_LockerCount WRITE set_LockerCount STORED false)
  QCEP_INTEGER_PROPERTY(LockerCount)

  Q_PROPERTY(double lockerRate READ get_LockerRate WRITE set_LockerRate STORED false)
  QCEP_DOUBLE_PROPERTY(LockerRate)

protected:
  QxrdStartupWindowPtr            m_StartupWindow;

private:
  void hideSplash();
  void setupTiffHandlers();

  QcepAllocatorPtr                m_Allocator;

  QTimer                          m_SplashTimer;
//  QxrdSplashScreenPtr             m_Splash;

  QxrdWelcomeWindowPtr            m_WelcomeWindow;

  QList<QxrdExperimentThreadPtr>  m_ExperimentThreads;
  QList<QxrdExperimentWPtr>       m_Experiments;
};

#endif // QXRDAPPCOMMON_H
