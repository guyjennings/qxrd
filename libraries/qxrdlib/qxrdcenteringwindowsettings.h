#ifndef QXRDCENTERINGWINDOWSETTINGS_H
#define QXRDCENTERINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdfilebrowsersettings-ptr.h"

class QXRD_EXPORT QxrdCenteringWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdCenteringWindowSettings(QString name);

  QxrdMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdFileBrowserSettingsPtr fileBrowserSettings();

private:
  QxrdFileBrowserSettingsPtr m_FileBrowserSettings;
};

Q_DECLARE_METATYPE(QxrdCenteringWindowSettings*)

#endif // QXRDCENTERINGWINDOWSETTINGS_H
