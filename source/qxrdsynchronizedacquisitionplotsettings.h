#ifndef QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H

#include "qxrdplotsettings.h"

class QxrdSynchronizedAcquisitionPlotSettings : public QxrdPlotSettings
{
  Q_OBJECT
public:
  explicit QxrdSynchronizedAcquisitionPlotSettings(QxrdSettingsSaverWPtr saver, QObject *parent = 0);

signals:

public slots:

};

typedef QSharedPointer<QxrdSynchronizedAcquisitionPlotSettings> QxrdSynchronizedAcquisitionPlotSettingsPtr;
typedef QWeakPointer<QxrdSynchronizedAcquisitionPlotSettings>   QxrdSynchronizedAcquisitionPlotSettingsWPtr;

#endif // QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H
