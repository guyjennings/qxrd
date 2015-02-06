#ifndef QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H
#define QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H

#include <QObject>
#include "qxrdimageplotsettings.h"

class QxrdDistortionCorrectionPlotSettings : public QxrdImagePlotSettings
{
  Q_OBJECT
public:
  explicit QxrdDistortionCorrectionPlotSettings(QxrdSettingsSaverWPtr saver, QObject *parent);

signals:

public slots:

};

typedef QSharedPointer<QxrdDistortionCorrectionPlotSettings> QxrdDistortionCorrectionPlotSettingsPtr;
typedef QWeakPointer<QxrdDistortionCorrectionPlotSettings>   QxrdDistortionCorrectionPlotSettingsWPtr;

#endif // QXRDDISTORTIONCORRECTIONPLOTSETTINGS_H
