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
#include "qcepdataobject-ptr.h"
#include "qxrdplugininfomodel-ptr.h"

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
  QString applicationDescription();
  QIcon   applicationIcon();

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
