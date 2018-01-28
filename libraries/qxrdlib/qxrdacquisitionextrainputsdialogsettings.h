#ifndef QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H
#define QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdacquisitionextrainputsplotsettings-ptr.h"
#include "qxrdacquisitionextrainputsdialogsettings-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExtraInputsDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdAcquisitionExtraInputsDialogSettings(QString name);
  static QxrdAcquisitionExtraInputsDialogSettingsPtr newAcquisitionExtraInputsDialogSettings();
  
public:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  QxrdAcquisitionExtraInputsPlotSettingsWPtr plotSettings() const;

signals:
  
public slots:

private:
  QMutex                                    m_Mutex;
  QxrdAcquisitionExtraInputsPlotSettingsPtr m_AcquisitionExtraInputsPlotSettings;
};

#endif // QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H
