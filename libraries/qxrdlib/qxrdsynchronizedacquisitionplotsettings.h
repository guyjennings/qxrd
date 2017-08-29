#ifndef QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepplotsettings.h"
#include "qxrdsynchronizedacquisitionplotsettings-ptr.h"

class QXRD_EXPORT QxrdSynchronizedAcquisitionPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSynchronizedAcquisitionPlotSettings(QString name);
  static QxrdSynchronizedAcquisitionPlotSettingsPtr newSynchronizedAcquisitionPlotSettings();

signals:

public slots:

};

Q_DECLARE_METATYPE(QxrdSynchronizedAcquisitionPlotSettings*)

#endif // QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H
