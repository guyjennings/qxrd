#ifndef QXRDSETTINGSSAVERTHREAD_H
#define QXRDSETTINGSSAVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdsettingssaver.h"
#include "qxrdthread.h"

class QxrdApplication;

class QxrdSettingsSaverThread : public QxrdThread
{
  Q_OBJECT;
public:
  QxrdSettingsSaverThread(QxrdApplication *app);

  QxrdSettingsSaver *settingsSaver();
  void shutdown();

protected:
  void run();

private:
  QxrdApplication   *m_Application;
  QxrdSettingsSaver *m_SettingsSaver;
};

#endif // QXRDSETTINGSSAVERTHREAD_H
