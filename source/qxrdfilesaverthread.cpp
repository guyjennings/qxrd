#include "qxrdfilesaverthread.h"

#include "qxrdfilesaver.h"
#include "qxrdacquisition.h"
#include "qxrdintegrateddata.h"

QxrdFileSaverThread::QxrdFileSaverThread(QxrdAllocator *allocator)
  : QThread(),
    m_FileSaver(NULL),
    m_Allocator(NULL)
{
  qRegisterMetaType<QxrdDoubleImageDataPtr>("QxrdDoubleImageDataPtr");
  qRegisterMetaType<QxrdInt32ImageDataPtr>("QxrdInt32ImageDataPtr");
  qRegisterMetaType<QxrdInt16ImageDataPtr>("QxrdInt16ImageDataPtr");
  qRegisterMetaType<QxrdMaskDataPtr>("QxrdMaskDataPtr");
  qRegisterMetaType<QxrdIntegratedDataPtr>("QxrdIntegratedDataPtr");
  qRegisterMetaType<FILE*>("FILE*");
  qRegisterMetaType< QVector<double> >("QVector<double>");

  m_Allocator.fetchAndStoreOrdered(allocator);
}

QxrdFileSaverThread::~QxrdFileSaverThread()
{
  shutdown();

  delete m_FileSaver;
}

void QxrdFileSaverThread::run()
{
  m_FileSaver.fetchAndStoreOrdered(new QxrdFileSaver(m_Allocator));

  connect(m_FileSaver, SIGNAL(printMessage(QDateTime,QString)), this, SIGNAL(printMessage(QDateTime,QString)));
  connect(m_FileSaver, SIGNAL(statusMessage(QDateTime,QString)), this, SIGNAL(statusMessage(QDateTime,QString)));
  connect(m_FileSaver, SIGNAL(criticalMessage(QDateTime,QString)), this, SIGNAL(criticalMessage(QDateTime,QString)));

  int rc = exec();

//  printf("File saver thread terminated with rc %d\n", rc);
}

void QxrdFileSaverThread::shutdown()
{
  exit();

  wait();
}

QxrdFileSaver *QxrdFileSaverThread::fileSaver() const
{
  while (m_FileSaver == NULL) {
    QThread::msleep(500);
  }

  return m_FileSaver;
}

void QxrdFileSaverThread::setProcessor(QxrdDataProcessor *proc)
{
  fileSaver() -> setProcessor(proc);
}

void QxrdFileSaverThread::setAcquisition(QxrdAcquisition *acq)
{
  fileSaver() -> setAcquisition(acq);
}

void QxrdFileSaverThread::saveData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
                                 Q_ARG(QString,name),
                                 Q_ARG(QxrdDoubleImageDataPtr,image),
                                 Q_ARG(QxrdMaskDataPtr,overflow),
                                 Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveData failed\n");
  }
}

void QxrdFileSaverThread::saveData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
                                 Q_ARG(QString,name),
                                 Q_ARG(QxrdInt32ImageDataPtr,image),
                                 Q_ARG(QxrdMaskDataPtr,overflow),
                                 Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveData failed\n");
  }
}

void QxrdFileSaverThread::saveData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
                                 Q_ARG(QString,name),
                                 Q_ARG(QxrdInt16ImageDataPtr,image),
                                 Q_ARG(QxrdMaskDataPtr,overflow),
                                 Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveData failed\n");
  }
}

void QxrdFileSaverThread::saveData(QString name, QxrdMaskDataPtr image, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
                                 Q_ARG(QString,name),
                                 Q_ARG(QxrdMaskDataPtr,image),
                                 Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveData failed\n");
  }
}

void QxrdFileSaverThread::saveRawData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveRawData",
                                 Q_ARG(QString,name),
                                 Q_ARG(QxrdInt32ImageDataPtr,image),
                                 Q_ARG(QxrdMaskDataPtr,overflow),
                                 Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveRawData failed\n");
  }
}

void QxrdFileSaverThread::saveRawData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveRawData",
                                 Q_ARG(QString,name),
                                 Q_ARG(QxrdInt16ImageDataPtr,image),
                                 Q_ARG(QxrdMaskDataPtr,overflow),
                                 Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveRawData failed\n");
  }
}

void QxrdFileSaverThread::saveTextData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveTextData",
                                 Q_ARG(QString,name),
                                 Q_ARG(QxrdDoubleImageDataPtr,image),
                                 Q_ARG(QxrdMaskDataPtr,overflow),
                                 Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveTextData failed\n");
  }
}

void QxrdFileSaverThread::writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "writeOutputScan",
                                 Q_ARG(FILE*, logFile),
                                 Q_ARG(QxrdIntegratedDataPtr, data))) {
    printf("QxrdFileSaverThread::writeOutputScan failed\n");
  }
}

void QxrdFileSaverThread::writeOutputScan(QString dir, QxrdIntegratedDataPtr data)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "writeOutputScan",
                                 Q_ARG(QString, dir),
                                 Q_ARG(QxrdIntegratedDataPtr, data))) {
    printf("QxrdFileSaverThread::writeOutputScan failed\n");
  }
}
