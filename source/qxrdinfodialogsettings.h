#ifndef QXRDINFODIALOGSETTINGS_H
#define QXRDINFODIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdInfoDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdInfoDialogSettings(QString name);
  QxrdInfoDialogSettings();
};

Q_DECLARE_METATYPE(QxrdInfoDialogSettings*)

#endif // QXRDINFODIALOGSETTINGS_H
