#ifndef QXRDGLOBALSETTINGS_H
#define QXRDGLOBALSETTINGS_H

#include "qcepmacros.h"

#include <QSettings>
class QxrdApplication;

class QxrdGlobalSettings : public QSettings
{
  Q_OBJECT

public:
  QxrdGlobalSettings(QxrdApplication *app);
  QxrdGlobalSettings(const QString &path, QSettings::Format format = QSettings::IniFormat, QObject *parent=0);
  ~QxrdGlobalSettings();

private:
  QxrdApplication *m_Application;
};

#endif
