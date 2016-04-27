#ifndef QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H

#include "qcepplotsettings.h"

class QxrdSynchronizedAcquisitionPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  explicit QxrdSynchronizedAcquisitionPlotSettings(QcepObjectWPtr parent);

signals:

public slots:

};

typedef QSharedPointer<QxrdSynchronizedAcquisitionPlotSettings> QxrdSynchronizedAcquisitionPlotSettingsPtr;
typedef QWeakPointer<QxrdSynchronizedAcquisitionPlotSettings>   QxrdSynchronizedAcquisitionPlotSettingsWPtr;

#endif // QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H
