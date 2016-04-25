#include "qxrddebug.h"
#include "qxrdfilesaverthread.h"

#include "qxrdfilesaver.h"
#include "qxrdacquisition.h"
#include "qcepintegrateddata.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"

QxrdFileSaverThread::QxrdFileSaverThread(QcepObjectWPtr parent,
                                         QcepAllocatorWPtr allocator)
  : QxrdThread(parent),
    m_FileSaver(),
    m_Allocator()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaverThread::QxrdFileSaverThread(%p)\n", this);
  }

  qRegisterMetaType<QcepDoubleImageDataPtr>("QcepDoubleImageDataPtr");
  qRegisterMetaType<QcepInt32ImageDataPtr>("QcepInt32ImageDataPtr");
  qRegisterMetaType<QcepInt16ImageDataPtr>("QcepInt16ImageDataPtr");
  qRegisterMetaType<QcepMaskDataPtr>("QcepMaskDataPtr");
  qRegisterMetaType<QcepIntegratedDataPtr>("QcepIntegratedDataPtr");
  qRegisterMetaType<FILE*>("FILE*");
  qRegisterMetaType< QVector<double> >("QVector<double>");

  m_Allocator = allocator;

  setObjectName("fileSaver");
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
  if (qcepDebug(DEBUG_THREADS)) {
    printf("File Saver Thread Started\n");
  }

  m_FileSaver = QxrdFileSaverPtr(new QxrdFileSaver(m_Allocator));

  int rc = exec();

  m_FileSaver = QxrdFileSaverPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("File Saver Thread Terminated with rc %d\n", rc);
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
    if (m_FileSaver) return m_FileSaver;

    QThread::msleep(50);
  }

  return QxrdFileSaverPtr();
}
