#ifndef QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H

#include "qcepplotsettings.h"

class QxrdSynchronizedAcquisitionPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSynchronizedAcquisitionPlotSettings(QString name);
  QxrdSynchronizedAcquisitionPlotSettings();

signals:

public slots:

};

Q_DECLARE_METATYPE(QxrdSynchronizedAcquisitionPlotSettings*)

#endif // QXRDSYNCHRONIZEDACQUISITIONPLOTSETTINGS_H
