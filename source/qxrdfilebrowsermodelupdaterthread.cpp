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
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileBrowserModelUpdaterThread::QxrdFileBrowserModelUpdaterThread(%p)\n", this);
  }

  setObjectName("updater");
}

QxrdFileBrowserModelUpdaterThread::~QxrdFileBrowserModelUpdaterThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileBrowserModelUpdaterThread::~QxrdFileBrowserModelUpdaterThread(%p)\n", this);
  }
}

void QxrdFileBrowserModelUpdaterThread::setModel(QxrdFileBrowserModelWPtr model)
{
  m_Model = model;
}

void QxrdFileBrowserModelUpdaterThread::shutdown()
{
//  INVOKE_CHECK(QMetaObject::invokeMethod(m_Updater.data(),"shutdown",Qt::QueuedConnection));
  exit();

  wait();
}

void QxrdFileBrowserModelUpdaterThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Browser Model Updater Thread Started\n");
  }

  m_Updater = QxrdFileBrowserModelUpdaterPtr(
        new QxrdFileBrowserModelUpdater(m_Model, NULL),
        &QObject::deleteLater);

  int rc = exec();

  m_Updater = QxrdFileBrowserModelUpdaterPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Browser Model Updater Thread Terminated with rc %d\n", rc);
  }
}

QxrdFileBrowserModelUpdaterPtr QxrdFileBrowserModelUpdaterThread::updater() const
{
  while (isRunning()) {
    if (m_Updater) return m_Updater;

    QThread::msleep(50);
  }

  return QxrdFileBrowserModelUpdaterPtr();
}
