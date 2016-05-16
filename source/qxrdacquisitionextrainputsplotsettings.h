#ifndef QXRDACQUISITIONEXTRAINPUTSPLOTSETTINGS_H
#define QXRDACQUISITIONEXTRAINPUTSPLOTSETTINGS_H

#include "qcepplotsettings.h"

class QxrdAcquisitionExtraInputsPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdAcquisitionExtraInputsPlotSettings(QString name);
  QxrdAcquisitionExtraInputsPlotSettings();
};

Q_DECLARE_METATYPE(QxrdAcquisitionExtraInputsPlotSettings*)

#endif // QXRDACQUISITIONEXTRAINPUTSPLOTSETTINGS_H
