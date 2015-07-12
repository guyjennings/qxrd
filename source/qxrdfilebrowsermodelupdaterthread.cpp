#include "qxrddebug.h"
#include "qxrdfilebrowsermodelupdaterthread.h"
#include "qxrdfilebrowsermodel.h"
#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"

QxrdFileBrowserModelUpdaterThread::QxrdFileBrowserModelUpdaterThread() :
  QxrdThread(),
  m_Model(),
  m_Updater()
{
}

QxrdFileBrowserModelUpdaterThread::~QxrdFileBrowserModelUpdaterThread()
{
  shutdown();
}

void QxrdFileBrowserModelUpdaterThread::setModel(QxrdFileBrowserModelWPtr model)
{
  m_Model = model;
}

void QxrdFileBrowserModelUpdaterThread::shutdown()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Updater.data(),"shutdown",Qt::QueuedConnection));

  wait();
}

void QxrdFileBrowserModelUpdaterThread::run()
{
  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Browser Model Updater Thread");
  }

  QxrdFileBrowserModelUpdaterPtr p =
      QxrdFileBrowserModelUpdaterPtr(
        new QxrdFileBrowserModelUpdater(m_Model, NULL));

  int rc = -1;

  if (p) {
    m_Mutex.lock();
    m_Updater = p;
    m_Mutex.unlock();

    rc = exec();
  }

  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Browser Model Updater Thread Terminated with rc %1").arg(rc));
  }
}

QxrdFileBrowserModelUpdaterPtr QxrdFileBrowserModelUpdaterThread::updater() const
{
  while (isRunning()) {
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_Updater) return m_Updater;
    }

    QThread::msleep(50);
  }

  return QxrdFileBrowserModelUpdaterPtr();
}
