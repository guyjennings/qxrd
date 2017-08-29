#ifndef QXRDSLICEPLOTSETTINGS_H
#define QXRDSLICEPLOTSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepplotsettings.h"
#include "qxrdsliceplotsettings-ptr.h"

class QXRD_EXPORT QxrdSlicePlotSettings : public QcepPlotSettings
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
