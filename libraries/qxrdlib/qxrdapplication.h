#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"

#include "qcepapplication.h"
#include <QSettings>
#include "qxrdresponsetimer.h"
#include "qxrdexperiment.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdwelcomewindow-ptr.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrddetectorplugininterface-ptr.h"
#include "qxrdsplashscreen-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qxrdapplicationsettings-ptr.h"
#include "qxrdplugininfomodel-ptr.h"

class QXRD_EXPORT QxrdApplication : public QcepApplication
{
  Q_OBJECT

public:
  QxrdApplication(int &argc, char **argv);
  virtual ~QxrdApplication();

  bool init(int &argc, char **argv);

  void loadPlugins();

  QxrdNIDAQPluginInterfacePtr    nidaqPlugin();
  QxrdDetectorPluginInterfacePtr simulatedDetectorPlugin();
  QxrdDetectorPluginInterfacePtr perkinElmerDetectorPlugin();
  QxrdDetectorPluginInterfacePtr dexelaDetectorPlugin();
  QxrdDetectorPluginInterfacePtr pilatusDetectorPlugin();
  QxrdDetectorPluginInterfacePtr areaDetectorPlugin();
  QxrdDetectorPluginInterfacePtr fileWatcherPlugin();
  QxrdDetectorPluginInterfaceWPtr detectorPlugin(int detType);

  void tiffWarning(const char* module, const char *msg);
  void tiffError(const char* module, const char *msg);

//  static QxrdApplicationPtr application();

  bool event(QEvent *ev);

//  QWidget* window();

  void readDefaultSettings();
  void writeDefaultSettings();
  void appendRecentExperiment(QString path);

  QString normalizeExperimentName(QString filename);
  void setNewExperimentSettings(QSettings &settings, int type, QString filename);

  QxrdApplicationSettingsWPtr settings();

public slots:
  void finish();

  void createNewExperiment();
  void chooseExistingExperiment();
  void openRecentExperiment(QString path);

  void activateExperiment(QString path);
  void openExperiment(QString path);
  void openExperiment2(QString path);
  void closeExperiment(QxrdExperimentWPtr expw);

  void openFile(QString filePath);
  void openWatcher(QString pattern);

  void openedExperiment(QxrdExperimentThreadWPtr expwthr);
  void closedExperiment(QxrdExperimentThreadWPtr expwthr);
  QList<QxrdExperimentWPtr> &experiments();

  QxrdExperimentPtr experiment(int i);

  void openWelcomeWindow();
  void closeWelcomeWindow();

  void doAboutQxrd();
  void doOpenQXRDWebPage();
  void doOpenURL(QString url);

  void possiblyQuit();
  void editGlobalPreferences();
  void debugChanged(qint64 newValue);

  void onAutoSaveTimer();

  void readSettings();
  void writeSettings();

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
  void printLine(QString msg);

  void processEventCounter();

  QString homePath();
  QString currentPath();
  QString rootPath();

  void lockerTimerElapsed();

  virtual void setDefaultObjectData(QcepDataObject *obj);


  QxrdPluginInfoModelWPtr pluginInfo();

public:
  bool wantToQuit();

  void incLockerCount();

private slots:
  void hideSplash();

private:

  void setupTiffHandlers();

private:
  QcepObjectNamer      m_ObjectNamer;
//  QcepSettingsSaverPtr m_AppSaver;
  QTimer               m_AutoSaveTimer;

private:
  QxrdApplicationSettingsPtr      m_ApplicationSettings;

  QList<QxrdExperimentThreadPtr>  m_ExperimentThreads;
  QList<QxrdExperimentWPtr>       m_Experiments;

  QTimer                          m_SplashTimer;
  QxrdSplashScreenPtr             m_Splash;

  QxrdWelcomeWindowPtr            m_WelcomeWindow;
  QxrdNIDAQPluginInterfacePtr     m_NIDAQPlugin;
  QxrdDetectorPluginInterfacePtr  m_SimulatedDetectorPlugin;
  QxrdDetectorPluginInterfacePtr  m_PerkinElmerDetectorPlugin;
  QxrdDetectorPluginInterfacePtr  m_DexelaPlugin;
  QxrdDetectorPluginInterfacePtr  m_PilatusDetectorPlugin;
  QxrdDetectorPluginInterfacePtr  m_AreaDetectorPlugin;
  QxrdDetectorPluginInterfacePtr  m_FileWatcherPlugin;

  QxrdResponseTimer              *m_ResponseTimer;

  QMutex                          m_SettingsMutex;

  QTimer                          m_LockerTimer;
  QTime                           m_LastLockerTime;
  int                             m_LastLockerCount;

//  QVector<QxrdDetectorPluginInterfacePtr> m_DetectorPlugins;

  QxrdPluginInfoModelPtr          m_PluginInfo;
};

#endif
