#ifndef QXRDGLOBALSETTINGS_H
#define QXRDGLOBALSETTINGS_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include <QSettings>
#include "qxrdapplication-ptr.h"

class QxrdGlobalSettings : public QSettings
{
  Q_OBJECT

public:
  QxrdGlobalSettings(QxrdApplication *app);
  QxrdGlobalSettings(const QString &path, QSettings::Format format = QSettings::IniFormat, QcepObject *parent=0);
  ~QxrdGlobalSettings();

private:
  QxrdApplication *m_Application;
};

#endif
