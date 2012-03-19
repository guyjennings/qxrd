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

#endif // QXRDINFODIALOGSETTINGS_H
