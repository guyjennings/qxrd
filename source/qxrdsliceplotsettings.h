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

#endif // QXRDSLICEPLOTSETTINGS_H
