#ifndef QXRDWATCHERWINDOWSETTINGS_H
#define QXRDWATCHERWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdwatcher-ptr.h"
#include "qxrdfilebrowsersettings-ptr.h"
#include "qxrdimageplotwidgetsettings-ptr.h"
#include "qxrdcenteringplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdWatcherWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdWatcherWindowSettings(QString name);

  void initialize(QxrdApplicationWPtr app,
                  QxrdExperimentWPtr  expt,
                  QxrdAcquisitionWPtr acqw,
                  QxrdProcessorWPtr   procw);

  QxrdMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdFileBrowserSettingsPtr          fileBrowserSettings();
  QxrdImagePlotWidgetSettingsPtr      imagePlotWidgetSettings();
  QxrdCenteringPlotWidgetSettingsPtr  centeringPlotWidgetSettings();

protected:
  QxrdWatcherWPtr  m_Watcher;

private:
  QxrdFileBrowserSettingsPtr          m_FileBrowserSettings;
  QxrdImagePlotWidgetSettingsPtr      m_ImagePlotWidgetSettings;
  QxrdCenteringPlotWidgetSettingsPtr  m_CenteringPlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdWatcherWindowSettings*)

#endif // QXRDWATCHERWINDOWSETTINGS_H
