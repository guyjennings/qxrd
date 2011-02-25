#ifndef QXRDSETTINGSSAVERTHREAD_H
#define QXRDSETTINGSSAVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdsettingssaver.h"
#include "qxrdforwardtypes.h"
#include <QThread>

class QxrdSettingsSaverThread : public QThread
{
  Q_OBJECT;
public:
  QxrdSettingsSaverThread(QxrdApplicationPtr app);

  QxrdSettingsSaver *settingsSaver();

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

protected:
  void run();

private:
  QxrdApplicationPtr   m_Application;
  QxrdSettingsSaverPtr m_SettingsSaver;
};

#endif // QXRDSETTINGSSAVERTHREAD_H
