#ifndef QXRDCENTERINGWINDOWSETTINGS_H
#define QXRDCENTERINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdfilebrowsersettings-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"
#include "qxrdcenteringplotwidgetsettings-ptr.h"
#include "qxrdintegratedplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdCenteringWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdCenteringWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdFileBrowserSettingsPtr          fileBrowserSettings();
  QcepImagePlotWidgetSettingsPtr      imagePlotWidgetSettings();
  QxrdCenteringPlotWidgetSettingsPtr  centeringPlotWidgetSettings();
  QxrdIntegratedPlotWidgetSettingsPtr integratedPlotWidgetSettings();

private:
  QxrdFileBrowserSettingsPtr          m_FileBrowserSettings;
  QcepImagePlotWidgetSettingsPtr      m_ImagePlotWidgetSettings;
  QxrdCenteringPlotWidgetSettingsPtr  m_CenteringPlotWidgetSettings;
  QxrdIntegratedPlotWidgetSettingsPtr m_IntegratedPlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdCenteringWindowSettings*)

#endif // QXRDCENTERINGWINDOWSETTINGS_H
