#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdsynchronizedacquisitionplotsettings.h"

class QxrdSynchronizedAcquisitionDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdSynchronizedAcquisitionDialogSettings(QcepObjectWPtr parent);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QxrdSynchronizedAcquisitionPlotSettingsWPtr synchronizedAcquisitionPlotSettings();
signals:

public slots:

private:
  QMutex              m_Mutex;
  QxrdSynchronizedAcquisitionPlotSettingsPtr m_SynchronizedAcquisitionPlotSettings;
};

typedef QSharedPointer<QxrdSynchronizedAcquisitionDialogSettings> QxrdSynchronizedAcquisitionDialogSettingsPtr;
typedef QWeakPointer<QxrdSynchronizedAcquisitionDialogSettings>   QxrdSynchronizedAcquisitionDialogSettingsWPtr;

#endif // QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
