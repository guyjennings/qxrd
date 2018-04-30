#ifndef QXRDACQUISITIONWINDOWSETTINGS_H
#define QXRDACQUISITIONWINDOWSETTINGS_H

#include "qxrdmainwindowsettings.h"
#include "qcepfilebrowsersettings-ptr.h"

class QXRD_EXPORT QxrdAcquisitionWindowSettings : public QxrdMainWindowSettings
{
  Q_OBJECT

private:
  typedef QxrdMainWindowSettings inherited;

public:
  Q_INVOKABLE QxrdAcquisitionWindowSettings(QString name);

  void initialize(QcepObjectWPtr parent);

  QcepMainWindowPtr newWindow();

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  QcepFileBrowserSettingsPtr fileBrowserSettings();

private:
  QcepFileBrowserSettingsPtr m_FileBrowserSettings;
};

Q_DECLARE_METATYPE(QxrdAcquisitionWindowSettings*)

#endif // QXRDACQUISITIONWINDOWSETTINGS_H
