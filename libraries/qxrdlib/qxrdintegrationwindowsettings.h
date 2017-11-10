#ifndef QXRDINTEGRATIONWINDOWSETTINGS_H
#define QXRDINTEGRATIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdfilebrowsersettings-ptr.h"

class QXRD_EXPORT QxrdIntegrationWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdIntegrationWindowSettings(QString name);

  QxrdMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdFileBrowserSettingsPtr fileBrowserSettings();

private:
  QxrdFileBrowserSettingsPtr m_FileBrowserSettings;
};

Q_DECLARE_METATYPE(QxrdIntegrationWindowSettings*)

#endif // QXRDINTEGRATIONWINDOWSETTINGS_H
