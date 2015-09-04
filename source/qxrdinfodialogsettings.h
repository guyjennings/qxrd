#ifndef QXRDINFODIALOGSETTINGS_H
#define QXRDINFODIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdInfoDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdInfoDialogSettings(QcepSettingsSaverWPtr saver, QcepObject *parent);
  
public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdInfoDialogSettings> QxrdInfoDialogSettingsPtr;
typedef QWeakPointer<QxrdInfoDialogSettings>   QxrdInfoDialogSettingsWPtr;

#endif // QXRDINFODIALOGSETTINGS_H
