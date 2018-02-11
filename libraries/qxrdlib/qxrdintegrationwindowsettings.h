#ifndef QXRDINTEGRATIONWINDOWSETTINGS_H
#define QXRDINTEGRATIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdfilebrowsersettings-ptr.h"
#include "qxrdimageplotwidgetsettings-ptr.h"
#include "qxrdintegratedplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdIntegrationWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  QxrdIntegrationWindowSettings(QString name, QString desc);

  QxrdMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdFileBrowserSettingsPtr          fileBrowserSettings();
  QxrdImagePlotWidgetSettingsPtr      imagePlotWidgetSettings();
  QxrdIntegratedPlotWidgetSettingsPtr integratedPlotWidgetSettings();

private:
  QxrdFileBrowserSettingsPtr          m_FileBrowserSettings;
  QxrdImagePlotWidgetSettingsPtr      m_ImagePlotWidgetSettings;
  QxrdIntegratedPlotWidgetSettingsPtr m_IntegratedPlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdIntegrationWindowSettings*)

#endif // QXRDINTEGRATIONWINDOWSETTINGS_H
