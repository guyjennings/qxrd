#ifndef QXRDWATCHERWINDOWSETTINGS_H
#define QXRDWATCHERWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdwatcher-ptr.h"
#include "qcepfilebrowsersettings-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"
#include "qxrdcenteringplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdWatcherWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdWatcherWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QcepFileBrowserSettingsPtr          fileBrowserSettings();
  QcepImagePlotWidgetSettingsPtr      imagePlotWidgetSettings();
  QxrdCenteringPlotWidgetSettingsPtr  centeringPlotWidgetSettings();

protected:
  QxrdWatcherWPtr  m_Watcher;

private:
  QcepFileBrowserSettingsPtr          m_FileBrowserSettings;
  QcepImagePlotWidgetSettingsPtr      m_ImagePlotWidgetSettings;
  QxrdCenteringPlotWidgetSettingsPtr  m_CenteringPlotWidgetSettings;

public:
  Q_PROPERTY(QString pattern READ get_Pattern WRITE set_Pattern)
  QCEP_STRING_PROPERTY(Pattern)
};

Q_DECLARE_METATYPE(QxrdWatcherWindowSettings*)

#endif // QXRDWATCHERWINDOWSETTINGS_H
