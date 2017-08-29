#ifndef QXRDSLICEPLOTSETTINGS_H
#define QXRDSLICEPLOTSETTINGS_H

#include "qcepplotsettings.h"
#include "qxrdsliceplotsettings-ptr.h"

class QxrdSlicePlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSlicePlotSettings(QString name);
  static QxrdSlicePlotSettingsPtr newSlicePlotSettings();

signals:
  
public slots:
  
};

Q_DECLARE_METATYPE(QxrdSlicePlotSettings*)

#endif // QXRDSLICEPLOTSETTINGS_H
