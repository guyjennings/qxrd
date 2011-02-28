#ifndef QXRDSETTINGSSAVERTHREAD_H
#define QXRDSETTINGSSAVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdsettingssaver.h"
#include <QThread>

class QxrdApplication;

class QxrdSettingsSaverThread : public QThread
{
  Q_OBJECT;
public:
  QxrdSettingsSaverThread(QxrdApplication *app);

  QxrdSettingsSaver *settingsSaver();

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

protected:
  void run();

private:
  QxrdApplication   *m_Application;
  QxrdSettingsSaver *m_SettingsSaver;
};

#endif // QXRDSETTINGSSAVERTHREAD_H
