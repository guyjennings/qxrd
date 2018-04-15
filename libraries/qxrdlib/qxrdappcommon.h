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
#include "qxrddebug.h"

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

  void shutdownDocuments();

  static QxrdAppCommonWPtr findApplication(QcepObjectWPtr p);


  void appendWatcher(QString patt);
  void appendPlugin(QString dir);

  void parseCommandLine();

  void tiffWarning(const char* module, const char *msg);
  void tiffError(const char* module, const char *msg);

  void openFile(QString filePath);
  void openWatcher(QString pattern);

  void openWelcomeWindow();
  void closeWelcomeWindow();

  void openRecentExperiment(QString path);
  void appendRecentExperiment(QString path);
  void closeExperiment(QxrdExperimentWPtr expw);


  Q_INVOKABLE virtual void openWindow(QxrdMainWindowSettingsWPtr set);

  virtual void doAboutQxrd();
  virtual void doOpenQXRDWebPage();
  virtual void doOpenURL(QString url);

  QcepExperimentPtr getFirstExperiment();

  void openedExperiment(QxrdExperimentThreadWPtr expwthr);
  void closedExperiment(QxrdExperimentThreadWPtr expwthr);
  QList<QxrdExperimentWPtr> &experiments();

  QcepExperimentPtr experiment(int i) const;

  Q_ENUM(QxrdDebugFlags)

public:
  Q_PROPERTY(QStringList watcherList READ get_WatcherList WRITE set_WatcherList STORED false)
  QCEP_STRING_LIST_PROPERTY(WatcherList)

  Q_PROPERTY(QStringList pluginList READ get_PluginList WRITE set_PluginList STORED false)
  QCEP_STRING_LIST_PROPERTY(PluginList)

  Q_PROPERTY(int    loadPlugins    READ get_LoadPlugins WRITE set_LoadPlugins STORED false)
  QCEP_INTEGER_PROPERTY(LoadPlugins)

  Q_PROPERTY(int    startDetectors READ get_StartDetectors WRITE set_StartDetectors STORED false)
  QCEP_INTEGER_PROPERTY(StartDetectors)

  Q_PROPERTY(QString currentDirectory READ get_CurrentDirectory WRITE set_CurrentDirectory)
  QCEP_STRING_PROPERTY(CurrentDirectory)

  Q_PROPERTY(int    fileBrowserLimit   READ get_FileBrowserLimit WRITE set_FileBrowserLimit)
  QCEP_INTEGER_PROPERTY(FileBrowserLimit)

protected:

private:
//  void hideSplash();
  void setupTiffHandlers();


//  QTimer                          m_SplashTimer;
//  QxrdSplashScreenPtr             m_Splash;

  QxrdWelcomeWindowPtr            m_WelcomeWindow;

  QList<QxrdExperimentThreadPtr>  m_ExperimentThreads;
  QList<QxrdExperimentWPtr>       m_Experiments;
};

#endif // QXRDAPPCOMMON_H
