#ifndef QXRDVIEWERSETTINGS_H
#define QXRDVIEWERSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include <QSettings>
#include "qxrdappcommon-ptr.h"

class QXRD_EXPORT QxrdViewerSettings : public QSettings
{
  Q_OBJECT

public:
  QxrdViewerSettings(QxrdAppCommon *app);
  QxrdViewerSettings(const QString &path, QSettings::Format format = QSettings::IniFormat, QcepObject *parent=0);
  ~QxrdViewerSettings();

private:
  QxrdAppCommon *m_Application;
};

#endif // QXRDVIEWERSETTINGS_H
