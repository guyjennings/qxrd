#ifndef QXRDSCRIPTDIALOGSETTINGS_H
#define QXRDSCRIPTDIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdsettingssaver-ptr.h"

class QxrdScriptDialogSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdScriptDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

typedef QSharedPointer<QxrdScriptDialogSettings> QxrdScriptDialogSettingsPtr;
typedef QWeakPointer<QxrdScriptDialogSettings>   QxrdScriptDialogSettingsWPtr;

#endif // QXRDSCRIPTDIALOGSETTINGS_H
