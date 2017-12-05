#ifndef QXRDSCRIPTDIALOGSETTINGS_H
#define QXRDSCRIPTDIALOGSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdscriptdialogsettings-ptr.h"

class QXRD_EXPORT QxrdScriptDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdScriptDialogSettings(QString name);
  static QxrdScriptDialogSettingsPtr newScriptDialogSettings();
};

Q_DECLARE_METATYPE(QxrdScriptDialogSettings*)

#endif // QXRDSCRIPTDIALOGSETTINGS_H
