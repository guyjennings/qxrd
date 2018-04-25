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
//  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileSaverThread::~QxrdFileSaverThread(%p)\n", this);
  }
}

void QxrdFileSaverThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("File Saver Thread Started\n");
  }

  m_FileSaver = QxrdFileSaverPtr(NEWPTR(QxrdFileSaver()));
  m_FileSaver -> initialize(sharedFromThis());

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
