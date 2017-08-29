#ifndef QXRDACQUISITIONEXTRAINPUTSPLOTSETTINGS_H
#define QXRDACQUISITIONEXTRAINPUTSPLOTSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepplotsettings.h"
#include "qxrdacquisitionextrainputsplotsettings-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExtraInputsPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdAcquisitionExtraInputsPlotSettings(QString name);
  static QxrdAcquisitionExtraInputsPlotSettingsPtr newAcquisitionExtraInputsPlotSettings();
};

Q_DECLARE_METATYPE(QxrdAcquisitionExtraInputsPlotSettings*)

#endif // QXRDACQUISITIONEXTRAINPUTSPLOTSETTINGS_H
