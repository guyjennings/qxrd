#ifndef QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H
#define QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdplotsettings.h"

class QxrdAcquisitionExtraInputsDialogSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionExtraInputsDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent);
  
public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QxrdPlotSettingsWPtr plotSettings() const;

signals:
  
public slots:

private:
  QxrdPlotSettingsPtr m_AcquisitionExtraInputsPlotSettings;
};

typedef QSharedPointer<QxrdAcquisitionExtraInputsDialogSettings> QxrdAcquisitionExtraInputsDialogSettingsPtr;
typedef QWeakPointer<QxrdAcquisitionExtraInputsDialogSettings>   QxrdAcquisitionExtraInputsDialogSettingsWPtr;

#endif // QXRDACQUISITIONEXTRAINPUTSDIALOGSETTINGS_H