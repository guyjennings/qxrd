#ifndef QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H
#define QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H

#include <QObject>
#include "qcepplotsettings.h"

class QxrdDistortionCorrectionPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDistortionCorrectionPlotSettings(QString name);
  QxrdDistortionCorrectionPlotSettings();
};

Q_DECLARE_METATYPE(QxrdDistortionCorrectionPlotSettings*)

#endif // QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H
