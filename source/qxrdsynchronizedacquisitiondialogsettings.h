#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdsynchronizedacquisitionplotsettings.h"

class QxrdSynchronizedAcquisitionDialogSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdSynchronizedAcquisitionDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent = 0);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QxrdSynchronizedAcquisitionPlotSettingsWPtr synchronizedAcquisitionPlotSettings();
signals:

public slots:

private:
  QxrdSynchronizedAcquisitionPlotSettingsPtr m_SynchronizedAcquisitionPlotSettings;
};

typedef QSharedPointer<QxrdSynchronizedAcquisitionDialogSettings> QxrdSynchronizedAcquisitionDialogSettingsPtr;
typedef QWeakPointer<QxrdSynchronizedAcquisitionDialogSettings>   QxrdSynchronizedAcquisitionDialogSettingsWPtr;

#endif // QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
