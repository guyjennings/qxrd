#ifndef QXRDINTEGRATIONWINDOWSETTINGS_H
#define QXRDINTEGRATIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdfilebrowsersettings-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"
#include "qxrdintegratedplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdIntegrationWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdIntegrationWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdFileBrowserSettingsPtr          fileBrowserSettings();
  QcepImagePlotWidgetSettingsPtr imagePlotWidgetSettings();
  QxrdIntegratedPlotWidgetSettingsPtr integratedPlotWidgetSettings();

private:
  QxrdFileBrowserSettingsPtr          m_FileBrowserSettings;
  QcepImagePlotWidgetSettingsPtr      m_ImagePlotWidgetSettings;
  QxrdIntegratedPlotWidgetSettingsPtr m_IntegratedPlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdIntegrationWindowSettings*)

#endif // QXRDINTEGRATIONWINDOWSETTINGS_H
