#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"

#include "qxrdappcommon.h"
#include <QSettings>
#include "qxrdresponsetimer.h"
#include "qxrdexperiment.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrddetectorplugininterface-ptr.h"
#include "qxrdsplashscreen-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qxrdapplicationsettings-ptr.h"
#include "qxrdplugininfomodel-ptr.h"

class QXRD_EXPORT QxrdApplication : public QxrdAppCommon
{
  Q_OBJECT

private:
  typedef QxrdAppCommon inherited;

public:
  explicit QxrdApplication(int &argc, char **argv);
  virtual ~QxrdApplication();
  void initialize(QObjectWPtr parent);

  //TODO: merge into 'initialize'
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
//  void openExperiment2(QString path);

//  void doAboutQxrd();
//  void doOpenQXRDWebPage();
//  void doOpenURL(QString url);

  void editGlobalPreferences();
  void debugChanged(qint64 newValue);

  void onAutoSaveTimer();

  void readSettings();
  void writeSettings();

  void doSavePreferences();
  void doLoadPreferences();

  void loadPreferences(QString path);
  void savePreferences(QString path);

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
  QxrdApplicationSettingsPtr settings();

public:
  void incLockerCount();

private:
  QString applicationDescription();

private:
  QcepObjectNamer                 m_ObjectNamer;
  QTimer                          m_AutoSaveTimer;

  QxrdNIDAQPluginInterfacePtr     m_NIDAQPlugin;
  QxrdDetectorPluginInterfacePtr  m_SimulatedDetectorPlugin;
  QxrdDetectorPluginInterfacePtr  m_PerkinElmerDetectorPlugin;
  QxrdDetectorPluginInterfacePtr  m_DexelaPlugin;
  QxrdDetectorPluginInterfacePtr  m_PilatusDetectorPlugin;
  QxrdDetectorPluginInterfacePtr  m_AreaDetectorPlugin;
  QxrdDetectorPluginInterfacePtr  m_FileWatcherPlugin;

//  QxrdResponseTimer              *m_ResponseTimer;

  QMutex                          m_SettingsMutex;

  QTimer                          m_LockerTimer;
  QTime                           m_LastLockerTime;
  int                             m_LastLockerCount;

//  QVector<QxrdDetectorPluginInterfacePtr> m_DetectorPlugins;

  QxrdPluginInfoModelPtr          m_PluginInfo;
};

#endif
