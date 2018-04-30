#ifndef QXRDINTEGRATIONWINDOWSETTINGS_H
#define QXRDINTEGRATIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qcepfilebrowsersettings-ptr.h"
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

  QcepFileBrowserSettingsPtr          fileBrowserSettings();
  QcepImagePlotWidgetSettingsPtr imagePlotWidgetSettings();
  QxrdIntegratedPlotWidgetSettingsPtr integratedPlotWidgetSettings();

private:
  QcepFileBrowserSettingsPtr          m_FileBrowserSettings;
  QcepImagePlotWidgetSettingsPtr      m_ImagePlotWidgetSettings;
  QxrdIntegratedPlotWidgetSettingsPtr m_IntegratedPlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdIntegrationWindowSettings*)

#endif // QXRDINTEGRATIONWINDOWSETTINGS_H
