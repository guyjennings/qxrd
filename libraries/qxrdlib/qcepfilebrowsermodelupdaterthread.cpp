#include "qcepdebug.h"
#include "qcepfilebrowsermodelupdaterthread.h"
#include "qcepfilebrowsermodel.h"
#include "qcepfilebrowsermodelupdater.h"

QcepFileBrowserModelUpdaterThread::QcepFileBrowserModelUpdaterThread(QString name) :
  inherited(name),
  m_Model(),
  m_Updater()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepFileBrowserModelUpdaterThread::QcepFileBrowserModelUpdaterThread(%p)\n", this);
  }
}

QcepFileBrowserModelUpdaterThread::~QcepFileBrowserModelUpdaterThread()
{
#ifndef QT_NO_DEBUG
  printf("Shutdown file browser model updater thread\n");
#endif

  shutdown();

#ifndef QT_NO_DEBUG
  printf("Deleting file browser model updater thread\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepFileBrowserModelUpdaterThread::~QcepFileBrowserModelUpdaterThread(%p)\n", this);
  }
}

void QcepFileBrowserModelUpdaterThread::setModel(QcepFileBrowserModelWPtr model)
{
  m_Model = model;
}

void QcepFileBrowserModelUpdaterThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Browser Model Updater Thread Started\n");
  }

  {
    QcepFileBrowserModelUpdaterPtr updater =
        QcepFileBrowserModelUpdaterPtr(
          NEWPTR(QcepFileBrowserModelUpdater("browserModelUpdater")));

    if (updater) {
      updater -> initialize(sharedFromThis());

      updater -> setBrowserModel(m_Model);
    }

    m_Updater = updater;
  }

  int rc = exec();

  m_Updater = QcepFileBrowserModelUpdaterPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Browser Model Updater Thread Terminated with rc %d\n", rc);
  }
}

void QcepFileBrowserModelUpdaterThread::haltUpdater()
{
  if (m_Updater) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_Updater.data(),
                                    &QcepFileBrowserModelUpdater::haltUpdater,
                                    Qt::BlockingQueuedConnection))
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_Updater.data(),
                                    "haltUpdater",
                                    Qt::BlockingQueuedConnection))
#endif
  }
}

QcepFileBrowserModelUpdaterPtr QcepFileBrowserModelUpdaterThread::updater() const
{
  while (isRunning()) {
    if (m_Updater) return m_Updater;

    msleep(50);
  }

  return QcepFileBrowserModelUpdaterPtr();
}
