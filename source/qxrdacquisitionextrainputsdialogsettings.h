#ifndef QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H
#define QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdacquisitionextrainputsplotsettings-ptr.h"

class QxrdAcquisitionExtraInputsDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdAcquisitionExtraInputsDialogSettings(QString name);
  QxrdAcquisitionExtraInputsDialogSettings();
  
public:
  virtual void addChildPtr(QcepObjectPtr child);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QxrdAcquisitionExtraInputsPlotSettingsWPtr plotSettings() const;

signals:
  
public slots:

private:
  QMutex                                    m_Mutex;
  QxrdAcquisitionExtraInputsPlotSettingsPtr m_AcquisitionExtraInputsPlotSettings;
};

#endif // QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H
