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
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

protected:
  void run();

private:
  QxrdApplicationPtr   m_Application;
  QxrdSettingsSaverPtr m_SettingsSaver;
};

#endif // QXRDSETTINGSSAVERTHREAD_H
