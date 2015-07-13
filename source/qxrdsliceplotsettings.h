#ifndef QXRDSLICEPLOTSETTINGS_H
#define QXRDSLICEPLOTSETTINGS_H

#include "qcepplotsettings.h"

class QxrdSlicePlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  explicit QxrdSlicePlotSettings(QcepSettingsSaverWPtr saver, QObject *parent);
  
signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdSlicePlotSettings> QxrdSlicePlotSettingsPtr;
typedef QWeakPointer<QxrdSlicePlotSettings>   QxrdSlicePlotSettingsWPtr;

#endif // QXRDSLICEPLOTSETTINGS_H
