#include "qxrdsettingssaverthread.h"
#include "qxrdapplication.h"

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
  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Settings Saver Thread");
  }

  m_SettingsSaver = new QxrdSettingsSaver(NULL, m_Application);

  int rc = exec();

  if (rc || qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Settings Saver Thread Terminated with rc %1").arg(rc));
  }
}

void QxrdSettingsSaverThread::shutdown()
{
  exit();

  wait();
}
