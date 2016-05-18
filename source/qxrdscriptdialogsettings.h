#ifndef QXRDSCRIPTDIALOGSETTINGS_H
#define QXRDSCRIPTDIALOGSETTINGS_H

#include "qcepserializableobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdScriptDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdScriptDialogSettings(QString name);
  QxrdScriptDialogSettings();
};

Q_DECLARE_METATYPE(QxrdScriptDialogSettings*)

#endif // QXRDSCRIPTDIALOGSETTINGS_H
