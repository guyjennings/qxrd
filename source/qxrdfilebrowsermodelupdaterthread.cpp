#include "qxrdfilebrowsermodelupdaterthread.h"
#include "qxrdfilebrowsermodel.h"
#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdapplication.h"

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

  m_Updater = p;

  int rc = exec();

  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Browser Model Updater Thread Terminated with rc %1").arg(rc));
  }
}

QxrdFileBrowserModelUpdaterPtr QxrdFileBrowserModelUpdaterThread::updater() const
{
  while (m_Updater == NULL) {
    QThread::msleep(50);
  }

  return m_Updater;
}
