#ifndef QXRDACQUISITIONWINDOWSETTINGS_H
#define QXRDACQUISITIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qxrdfilebrowsersettings-ptr.h"

class QXRD_EXPORT QxrdAcquisitionWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdAcquisitionWindowSettings(QString name);

  QxrdMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QxrdFileBrowserSettingsPtr fileBrowserSettings();

private:
  QxrdFileBrowserSettingsPtr m_FileBrowserSettings;
};

Q_DECLARE_METATYPE(QxrdAcquisitionWindowSettings*)

#endif // QXRDACQUISITIONWINDOWSETTINGS_H
