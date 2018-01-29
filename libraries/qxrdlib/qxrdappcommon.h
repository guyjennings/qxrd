#ifndef QXRDAPPCOMMON_H
#define QXRDAPPCOMMON_H

#include "qxrdlib_global.h"
#include "qcepproperty.h"
#include "qcepapplication.h"
#include "qxrdappcommonsettings-ptr.h"
#include "qxrdsplashscreen-ptr.h"
#include "qxrdwelcomewindow-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdmainwindowsettings-ptr.h"
#include <QTimer>

class QXRD_EXPORT QxrdAppCommon : public QcepApplication
{
  Q_OBJECT

private:
  typedef QcepApplication inherited;

public:
  QxrdAppCommon(int &argc, char **argv);
  virtual ~QxrdAppCommon();

  virtual bool init(int &argc, char **argv);
  virtual void finish();

  void parseCommandLine(bool wantFullOptions);

  Q_INVOKABLE void splashMessage(QString msg);

  void tiffWarning(const char* module, const char *msg);
  void tiffError(const char* module, const char *msg);

  void openFile(QString filePath);
  void openWatcher(QString pattern);

  void openWelcomeWindow();
  void closeWelcomeWindow();

  QxrdAppCommonSettingsPtr settings();
  void setSettings(QxrdAppCommonSettingsPtr set);

  virtual QString applicationDescription() = 0;

  virtual void openExperiment(QString path) = 0;
  virtual void openRecentExperiment(QString path);
  void appendRecentExperiment(QString path);
  void closeExperiment(QxrdExperimentWPtr expw);

  Q_INVOKABLE virtual void openWindow(QxrdMainWindowSettingsWPtr set);

  virtual void readSettings() = 0;
  virtual void writeSettings() = 0;

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

  QxrdExperimentPtr experiment(int i);

  void possiblyQuit();
  bool wantToQuit();

private:
  void hideSplash();
  void setupTiffHandlers();

  QxrdAppCommonSettingsPtr        m_ApplicationSettings;
  QTimer                          m_SplashTimer;
  QxrdSplashScreenPtr             m_Splash;

  QxrdWelcomeWindowPtr            m_WelcomeWindow;

  QList<QxrdExperimentThreadPtr>  m_ExperimentThreads;
  QList<QxrdExperimentWPtr>       m_Experiments;
};

#endif // QXRDAPPCOMMON_H
