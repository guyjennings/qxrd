#include "qxrdsettingssaverthread.h"

QxrdSettingsSaverThread::QxrdSettingsSaverThread(QxrdApplication *app) :
    QxrdThread(),
    m_Application(app),
    m_SettingsSaver(NULL)
{
}

QxrdSettingsSaver *QxrdSettingsSaverThread::settingsSaver()
{
  while (m_SettingsSaver == NULL) {
    QThread::msleep(50);
  }

  return m_SettingsSaver;
}

void QxrdSettingsSaverThread::run()
{
  m_SettingsSaver = new QxrdSettingsSaver(NULL, m_Application);

  connect(m_SettingsSaver, SIGNAL(printMessage(QString,QDateTime)),
          this,            SIGNAL(printMessage(QString,QDateTime)));
  connect(m_SettingsSaver, SIGNAL(statusMessage(QString,QDateTime)),
          this,            SIGNAL(statusMessage(QString,QDateTime)));
  connect(m_SettingsSaver, SIGNAL(criticalMessage(QString,QDateTime)),
          this,            SIGNAL(criticalMessage(QString,QDateTime)));

  exec();
}

void QxrdSettingsSaverThread::shutdown()
{
  exit();

  wait();
}
