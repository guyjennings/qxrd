#ifndef QXRDINFODIALOGSETTINGS_H
#define QXRDINFODIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdInfoDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdInfoDialogSettings(QcepObjectWPtr parent);
  
public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:
  
public slots:
  
private:
  QMutex              m_Mutex;
};

typedef QSharedPointer<QxrdInfoDialogSettings> QxrdInfoDialogSettingsPtr;
typedef QWeakPointer<QxrdInfoDialogSettings>   QxrdInfoDialogSettingsWPtr;

#endif // QXRDINFODIALOGSETTINGS_H
