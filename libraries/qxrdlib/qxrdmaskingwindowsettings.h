#ifndef QXRDMASKINGWINDOWSETTINGS_H
#define QXRDMASKINGWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdfilebrowsersettings-ptr.h"

class QXRD_EXPORT QxrdMaskingWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdMaskingWindowSettings(QString name);

  QxrdMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdFileBrowserSettingsPtr fileBrowserSettings();

private:
  QxrdFileBrowserSettingsPtr m_FileBrowserSettings;
};

Q_DECLARE_METATYPE(QxrdMaskingWindowSettings*)

#endif // QXRDMASKINGWINDOWSETTINGS_H
