#ifndef QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
#define QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdDistortionCorrectionDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDistortionCorrectionDialogSettings(QString name);
  QxrdDistortionCorrectionDialogSettings();
};

Q_DECLARE_METATYPE(QxrdDistortionCorrectionDialogSettings*)

#endif // QXRDDISTORTIONCORRECTIONDIALOGSETTINGS_H
