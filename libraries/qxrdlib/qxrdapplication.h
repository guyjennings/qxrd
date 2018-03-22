#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"

#include "qxrdappcommon.h"
#include <QSettings>
#include "qxrdexperiment.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qxrdplugininfomodel-ptr.h"
#include "qxrdnidaq-ptr.h"
#include "qxrddetectorplugin-ptr.h"

class QXRD_EXPORT QxrdApplication : public QxrdAppCommon
{
  Q_OBJECT

private:
  typedef QxrdAppCommon inherited;

public:
  explicit QxrdApplication(int &argc, char **argv);
  virtual ~QxrdApplication();
  void initializeRoot();

  void openStartupWindow();
  void closeStartupWindow();

  void loadPlugins();

  QxrdNIDAQPtr nidaqPlugin();
//  QxrdDetectorPluginInterface *simulatedDetectorPlugin();
//  QxrdDetectorPluginInterfacePtr perkinElmerDetectorPlugin();
//  QxrdDetectorPluginInterfacePtr dexelaDetectorPlugin();
//  QxrdDetectorPluginInterfacePtr pilatusDetectorPlugin();
//  QxrdDetectorPluginInterfacePtr areaDetectorPlugin();
//  QxrdDetectorPluginInterfacePtr fileWatcherPlugin();
  QxrdDetectorPluginPtr detectorPlugin(int detType);

  bool event(QEvent *ev);

  void readDefaultSettings();
  void writeDefaultSettings();

  QString normalizeExperimentName(QString filename);
  void setNewExperimentSettings(QSettings &settings, int type, QString filename);

public slots:
  void finish();

  void createNewExperiment();
  void chooseExistingExperiment();

  void openExperiment(QString path);

//  void doAboutQxrd();
//  void doOpenQXRDWebPage();
//  void doOpenURL(QString url);

  void editGlobalPreferences();

  void onAutoSaveTimer();

  void readApplicationSettings();
  void writeApplicationSettings();

  void doSavePreferences();
  void doLoadPreferences();

  void loadPreferences(QString path);
  void savePreferences(QString path);

  void logMessage(QString msg) const;
  void warningMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void printLine(QString msg) const;

  void processEventCounter();

  QString homePath();
  QString currentPath();
  QString rootPath();

  void lockerTimerElapsed();

  virtual void setDefaultObjectData(QcepDataObject *obj);


  QxrdPluginInfoModelWPtr pluginInfo();

public:
  void incLockerCount();

private:
  QString applicationName();
  QString applicationVersion();
  QString applicationDescription();
  QIcon   applicationIcon();

private:
  QcepObjectNamer                 m_ObjectNamer;
  QTimer                          m_AutoSaveTimer;

  QxrdNIDAQPtr                    m_NIDAQPlugin;
  QxrdDetectorPluginPtr           m_SimulatedDetectorPlugin;
  QxrdDetectorPluginPtr           m_PerkinElmerDetectorPlugin;
  QxrdDetectorPluginPtr           m_PilatusDetectorPlugin;
  QxrdDetectorPluginPtr           m_AreaDetectorPlugin;
  QxrdDetectorPluginPtr           m_FileWatcherPlugin;
  QxrdDetectorPluginPtr           m_DexelaPlugin;
  QxrdDetectorPluginPtr           m_AlliedVisionPlugin;

  QMutex                          m_SettingsMutex;

  QTimer                          m_LockerTimer;
  QTime                           m_LastLockerTime;
  int                             m_LastLockerCount;

//  QVector<QxrdDetectorPluginInterfacePtr> m_DetectorPlugins;

  QxrdPluginInfoModelPtr          m_PluginInfo;
};

#endif
