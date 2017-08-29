#ifndef QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H
#define QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H

#include "qxrdlib_global.h"
#include <QObject>
#include "qcepplotsettings.h"
#include "qxrddistortioncorrectionplotsettings-ptr.h"

class QXRD_EXPORT QxrdDistortionCorrectionPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDistortionCorrectionPlotSettings(QString name);
  static QxrdDistortionCorrectionPlotSettingsPtr newDistortionCorrectionPlotSettings();
};

Q_DECLARE_METATYPE(QxrdDistortionCorrectionPlotSettings*)

#endif // QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H
