#ifndef QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H
#define QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H

#include <QObject>
#include "qcepplotsettings.h"
#include "qxrddistortioncorrectionplotsettings-ptr.h"

class QxrdDistortionCorrectionPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDistortionCorrectionPlotSettings(QString name);
  static QxrdDistortionCorrectionPlotSettingsPtr newDistortionCorrectionPlotSettings();
};

Q_DECLARE_METATYPE(QxrdDistortionCorrectionPlotSettings*)

#endif // QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H
