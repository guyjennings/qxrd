#ifndef QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H
#define QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H

#include "qcepserializableobject.h"
#include "qcepproperty.h"
#include "qxrdacquisitionextrainputsplotsettings-ptr.h"
#include "qxrdacquisitionextrainputsdialogsettings-ptr.h"

class QxrdAcquisitionExtraInputsDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdAcquisitionExtraInputsDialogSettings(QString name);
  static QxrdAcquisitionExtraInputsDialogSettingsPtr newAcquisitionExtraInputsDialogSettings();
  
public:
  virtual void addChildPtr(QcepSerializableObjectPtr child);

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
