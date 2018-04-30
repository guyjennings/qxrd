#include "qxrddebug.h"
#include "qxrdfilesaverthread.h"

#include "qxrdfilesaver.h"
#include "qxrdacqcommon.h"
#include "qcepintegrateddata.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"

QxrdFileSaverThread::QxrdFileSaverThread(QString name)
  : inherited(name),
    m_FileSaver()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaverThread::QxrdFileSaverThread(%p)\n", this);
  }

  qRegisterMetaType<QcepDoubleImageDataPtr>("QcepDoubleImageDataPtr");
  qRegisterMetaType<QcepInt32ImageDataPtr>("QcepInt32ImageDataPtr");
  qRegisterMetaType<QcepInt16ImageDataPtr>("QcepInt16ImageDataPtr");
  qRegisterMetaType<QcepUInt32ImageDataPtr>("QcepUInt32ImageDataPtr");
  qRegisterMetaType<QcepUInt16ImageDataPtr>("QcepUInt16ImageDataPtr");
  qRegisterMetaType<QcepMaskDataPtr>("QcepMaskDataPtr");
  qRegisterMetaType<QcepIntegratedDataPtr>("QcepIntegratedDataPtr");
  qRegisterMetaType<FILE*>("FILE*");
  qRegisterMetaType< QVector<double> >("QVector<double>");
}

QxrdFileSaverThread::~QxrdFileSaverThread()
{
#ifndef QT_NO_DEBUG
  printf("Shutting down file saver thread\n");
#endif

  shutdown();

#ifndef QT_NO_DEBUG
  printf("Deleting file saver thread\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaverThread::~QxrdFileSaverThread(%p)\n", this);
  }
}

void QxrdFileSaverThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("File Saver Thread Started\n");
  }

  {
    QxrdFileSaverPtr sav = QxrdFileSaverPtr(NEWPTR(QxrdFileSaver()));

    if (sav) {
      sav -> initialize(sharedFromThis());
    }

    m_FileSaver = sav;
  }

  int rc = exec();

  m_FileSaver = QxrdFileSaverPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("File Saver Thread Terminated with rc %d\n", rc);
  }
}

QxrdFileSaverPtr QxrdFileSaverThread::fileSaver() const
{
  while (isRunning()) {
    if (m_FileSaver) return m_FileSaver;

    QcepThread::msleep(50);
  }

  return QxrdFileSaverPtr();
}

void QxrdFileSaverThread::haltFileSaver()
{
  if (m_FileSaver) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_FileSaver.data(),
                                    &QxrdFileSaver::haltFileSaver,
                                    Qt::BlockingQueuedConnection))
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(m_FileSaver.data(),
                                    "haltFileSaver",
                                    Qt::BlockingQueuedConnection))
#endif
  }
}
