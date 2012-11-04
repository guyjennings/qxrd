#include "qxrddebug.h"
#include "qxrdfilesaverthread.h"

#include "qxrdfilesaver.h"
#include "qxrdacquisition.h"
#include "qxrdintegrateddata.h"
#include "qxrdapplication.h"
#include "qxrdmutexlocker.h"

QxrdFileSaverThread::QxrdFileSaverThread(QxrdAllocatorWPtr allocator)
  : QxrdThread(),
    m_FileSaver(),
    m_Allocator()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaverThread::QxrdFileSaverThread(%p)\n", this);
  }

  qRegisterMetaType<QxrdDoubleImageDataPtr>("QxrdDoubleImageDataPtr");
  qRegisterMetaType<QxrdInt32ImageDataPtr>("QxrdInt32ImageDataPtr");
  qRegisterMetaType<QxrdInt16ImageDataPtr>("QxrdInt16ImageDataPtr");
  qRegisterMetaType<QxrdMaskDataPtr>("QxrdMaskDataPtr");
  qRegisterMetaType<QxrdIntegratedDataPtr>("QxrdIntegratedDataPtr");
  qRegisterMetaType<FILE*>("FILE*");
  qRegisterMetaType< QVector<double> >("QVector<double>");

  m_Allocator = allocator;
}

QxrdFileSaverThread::~QxrdFileSaverThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaverThread::~QxrdFileSaverThread(%p)\n", this);
  }
}

void QxrdFileSaverThread::run()
{
  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting File Saver Thread");
  }

  QxrdFileSaverPtr res(new QxrdFileSaver(m_Allocator));

  m_Mutex.lock();
  m_FileSaver = res;
  m_Mutex.unlock();

  int rc = exec();

  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("File Saver Thread Terminated with rc %1").arg(rc));
  }
}

void QxrdFileSaverThread::shutdown()
{
  exit();

  wait();
}

QxrdFileSaverPtr QxrdFileSaverThread::fileSaver() const
{
  while (isRunning()) {
    {
      QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_FileSaver) return m_FileSaver;
    }

    QThread::msleep(50);
  }

  return QxrdFileSaverPtr();
}
