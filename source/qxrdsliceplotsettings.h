#ifndef QXRDSLICEPLOTSETTINGS_H
#define QXRDSLICEPLOTSETTINGS_H

#include "qxrdplotsettings.h"

class QxrdSlicePlotSettings : public QxrdPlotSettings
{
  Q_OBJECT
public:
  explicit QxrdSlicePlotSettings(QxrdSettingsSaverWPtr saver, QObject *parent);
  
signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdSlicePlotSettings> QxrdSlicePlotSettingsPtr;
typedef QWeakPointer<QxrdSlicePlotSettings>   QxrdSlicePlotSettingsWPtr;

#endif // QXRDSLICEPLOTSETTINGS_H
