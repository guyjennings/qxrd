#ifndef QXRDINFODIALOGSETTINGS_H
#define QXRDINFODIALOGSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepserializableobject.h"
#include "qcepproperty.h"
#include "qxrdinfodialogsettings-ptr.h"

class QXRD_EXPORT QxrdInfoDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdInfoDialogSettings(QString name);
  static QxrdInfoDialogSettingsPtr newInfoDialogSettings();
};

Q_DECLARE_METATYPE(QxrdInfoDialogSettings*)

#endif // QXRDINFODIALOGSETTINGS_H
