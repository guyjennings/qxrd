#ifndef QXRDINFODIALOGSETTINGS_H
#define QXRDINFODIALOGSETTINGS_H

#include "qcepserializableobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdInfoDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdInfoDialogSettings(QString name);
  QxrdInfoDialogSettings();
};

Q_DECLARE_METATYPE(QxrdInfoDialogSettings*)

#endif // QXRDINFODIALOGSETTINGS_H
