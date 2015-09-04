#ifndef QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H
#define QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepplotsettings-ptr.h"

class QxrdAcquisitionExtraInputsDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionExtraInputsDialogSettings(QcepSettingsSaverWPtr saver, QcepObject *parent);
  
public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QcepPlotSettingsWPtr plotSettings() const;

signals:
  
public slots:

private:
  QcepPlotSettingsPtr m_AcquisitionExtraInputsPlotSettings;
};

typedef QSharedPointer<QxrdAcquisitionExtraInputsDialogSettings> QxrdAcquisitionExtraInputsDialogSettingsPtr;
typedef QWeakPointer<QxrdAcquisitionExtraInputsDialogSettings>   QxrdAcquisitionExtraInputsDialogSettingsWPtr;

#endif // QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H
