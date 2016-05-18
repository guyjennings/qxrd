#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qcepserializableobject.h"
#include "qxrdsynchronizedacquisitionplotsettings-ptr.h"

class QxrdSynchronizedAcquisitionDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSynchronizedAcquisitionDialogSettings(QString name);
  QxrdSynchronizedAcquisitionDialogSettings();

public:
  virtual void addChildPtr(QcepSerializableObjectPtr child);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QxrdSynchronizedAcquisitionPlotSettingsWPtr synchronizedAcquisitionPlotSettings();
signals:

public slots:

private:
  QMutex              m_Mutex;
  QxrdSynchronizedAcquisitionPlotSettingsPtr m_SynchronizedAcquisitionPlotSettings;
};

Q_DECLARE_METATYPE(QxrdSynchronizedAcquisitionDialogSettings*)

#endif // QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
