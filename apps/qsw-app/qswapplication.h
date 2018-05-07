#ifndef QSWAPPLICATION_H
#define QSWAPPLICATION_H

#include "qswlib_global.h"
#include "qcepapplication.h"
#include "qswmainwindow-ptr.h"
#include "qswexperimentthread-ptr.h"
#include "qswexperiment-ptr.h"
#include "qcepplotwidgetsettings-ptr.h"
#include "qcepproperty.h"
#include "qcepallocator-ptr.h"

class /*QSW_EXPORT*/ QswApplication : public QcepApplication
{
  Q_OBJECT

private:
  typedef QcepApplication inherited;

public:
  QswApplication(int &argc, char **argv);
  void initializeRoot();

  QString      applicationName();
  QString      applicationMnemonic();
  QString      applicationVersion();
  QString      applicationDescription();
  QIcon        applicationIcon();
  QSettingsPtr applicationSettings();

  void openExperiment(QString path);
  void openRecentExperiment(QString path);

  void editGlobalPreferences();
  void createNewExperiment();
  void chooseExistingExperiment();

  QcepExperimentPtr experiment(int i) const;

private:
  QswMainWindowPtr          m_MainWindow;
  QswExperimentThreadPtr    m_ExperimentThread;
  QswExperimentWPtr         m_Experiment;
  QcepPlotWidgetSettingsPtr m_PlotSettings;
};

#endif // QSWAPPLICATION_H
