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

  connect(m_SettingsSaver,             SIGNAL(printMessage(QString)), this,            SIGNAL(printMessage(QString)));
  connect(m_SettingsSaver,             SIGNAL(statusMessage(QString)), this,            SIGNAL(statusMessage(QString)));
  connect(m_SettingsSaver,             SIGNAL(criticalMessage(QString)), this,            SIGNAL(criticalMessage(QString)));

  exec();
}
