#include "qxrdsettingssaverthread.h"

QxrdSettingsSaverThread::QxrdSettingsSaverThread(QxrdApplicationPtr app) :
    QThread(),
    m_Application(app),
    m_SettingsSaver(NULL)
{
}

QxrdSettingsSaver *QxrdSettingsSaverThread::settingsSaver()
{
  while (m_SettingsSaver == NULL) {
    QThread::msleep(500);
  }

  return m_SettingsSaver;
}

void QxrdSettingsSaverThread::run()
{
  m_SettingsSaver = new QxrdSettingsSaver(NULL, m_Application);

  connect(m_SettingsSaver, SIGNAL(printMessage(QDateTime,QString)),
          this,            SIGNAL(printMessage(QDateTime,QString)));
  connect(m_SettingsSaver, SIGNAL(statusMessage(QDateTime,QString)),
          this,            SIGNAL(statusMessage(QDateTime,QString)));
  connect(m_SettingsSaver, SIGNAL(criticalMessage(QDateTime,QString)),
          this,            SIGNAL(criticalMessage(QDateTime,QString)));

  exec();
}
