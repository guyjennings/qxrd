#ifndef QXRDSCRIPTDIALOGSETTINGS_H
#define QXRDSCRIPTDIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdScriptDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdScriptDialogSettings(QcepSettingsSaverWPtr saver, QcepObjectWPtr parent);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:

public slots:

private:
  QMutex              m_Mutex;
};

typedef QSharedPointer<QxrdScriptDialogSettings> QxrdScriptDialogSettingsPtr;
typedef QWeakPointer<QxrdScriptDialogSettings>   QxrdScriptDialogSettingsWPtr;

#endif // QXRDSCRIPTDIALOGSETTINGS_H
