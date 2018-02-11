#ifndef QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
#define QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H

#include "qxrdlib_global.h"
#include <QObject>
#include "qcepproperty.h"
#include "qcepobject.h"
#include "qxrdsynchronizedacquisitionplotsettings-ptr.h"
#include "qxrdsynchronizedacquisitiondialogsettings-ptr.h"

class QXRD_EXPORT QxrdSynchronizedAcquisitionDialogSettings : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdSynchronizedAcquisitionDialogSettings(QString name);
  static QxrdSynchronizedAcquisitionDialogSettingsPtr newSynchronizedAcquisitionDialogSettings();
  void initialize(QcepObjectWPtr parent);

public:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  QxrdSynchronizedAcquisitionPlotSettingsWPtr synchronizedAcquisitionPlotSettings();
signals:

public slots:

private:
  QxrdSynchronizedAcquisitionPlotSettingsPtr m_SynchronizedAcquisitionPlotSettings;
};

Q_DECLARE_METATYPE(QxrdSynchronizedAcquisitionDialogSettings*)

#endif // QXRDSYNCHRONIZEDACQUISITIONDIALOGSETTINGS_H
