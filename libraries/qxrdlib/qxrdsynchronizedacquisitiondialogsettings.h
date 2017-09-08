#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H

#include "qxrdlib_global.h"
#include <QObject>
#include "qcepproperty.h"
#include "qcepserializableobject.h"
#include "qxrdsynchronizedacquisitionplotsettings-ptr.h"
#include "qxrdsynchronizedacquisitiondialogsettings-ptr.h"

class QXRD_EXPORT QxrdSynchronizedAcquisitionDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSynchronizedAcquisitionDialogSettings(QString name);
  static QxrdSynchronizedAcquisitionDialogSettingsPtr newSynchronizedAcquisitionDialogSettings();

public:
  virtual void addChildPtr(QcepObjectPtr child);

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
