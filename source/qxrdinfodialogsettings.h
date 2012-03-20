#ifndef QXRDINFODIALOGSETTINGS_H
#define QXRDINFODIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"

class QxrdInfoDialogSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdInfoDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent);
  
public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdInfoDialogSettings> QxrdInfoDialogSettingsPtr;
typedef QWeakPointer<QxrdInfoDialogSettings>   QxrdInfoDialogSettingsWPtr;

#endif // QXRDINFODIALOGSETTINGS_H
