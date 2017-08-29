#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qcepserializableobject.h"
#include "qxrdsynchronizedacquisitionplotsettings-ptr.h"
#include "qxrdsynchronizedacquisitiondialogsettings-ptr.h"

class QxrdSynchronizedAcquisitionDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSynchronizedAcquisitionDialogSettings(QString name);
  static QxrdSynchronizedAcquisitionDialogSettingsPtr newSynchronizedAcquisitionDialogSettings();

public:
  virtual void addChildPtr(QcepSerializableObjectPtr child);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  QxrdSynchronizedAcquisitionPlotSettingsWPtr synchronizedAcquisitionPlotSettings();
signals:

public slots:

private:
  QMutex              m_Mutex;
  QxrdSynchronizedAcquisitionPlotSettingsPtr m_SynchronizedAcquisitionPlotSettings;
};

Q_DECLARE_METATYPE(QxrdSynchronizedAcquisitionDialogSettings*)

#endif // QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
