#include "qxrdfilesaverthread.h"

#include "qxrdfilesaver.h"
#include "qxrdacquisition.h"
#include "qxrdintegrateddata.h"
#include "qxrdapplication.h"

QxrdFileSaverThread::QxrdFileSaverThread(QxrdAllocatorPtr allocator)
  : QxrdThread(),
    m_FileSaver(NULL),
    m_Allocator(NULL)
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
  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting File Saver Thread");
  }

  QxrdFileSaverPtr res(new QxrdFileSaver(m_Allocator));

  m_FileSaver = res;

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
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
  while (m_FileSaver == NULL) {
    QThread::msleep(50);
  }

  return m_FileSaver;
}

//void QxrdFileSaverThread::setProcessor(QxrdDataProcessorPtr proc)
//{
//  fileSaver() -> setProcessor(proc);
//}

//void QxrdFileSaverThread::setAcquisition(QxrdAcquisitionPtr acq)
//{
//  fileSaver() -> setAcquisition(acq);
//}

//void QxrdFileSaverThread::saveData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
//{
//  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
//                                 Q_ARG(QString,name),
//                                 Q_ARG(QxrdDoubleImageDataPtr,image),
//                                 Q_ARG(QxrdMaskDataPtr,overflow),
//                                 Q_ARG(int,canOverwrite))) {
//    g_Application->printMessage("QxrdFileSaverThread::saveData failed");
//  }
//}

//void QxrdFileSaverThread::saveData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
//{
//  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
//                                 Q_ARG(QString,name),
//                                 Q_ARG(QxrdInt32ImageDataPtr,image),
//                                 Q_ARG(QxrdMaskDataPtr,overflow),
//                                 Q_ARG(int,canOverwrite))) {
//    g_Application->printMessage("QxrdFileSaverThread::saveData failed");
//  }
//}

//void QxrdFileSaverThread::saveData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
//{
//  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
//                                 Q_ARG(QString,name),
//                                 Q_ARG(QxrdInt16ImageDataPtr,image),
//                                 Q_ARG(QxrdMaskDataPtr,overflow),
//                                 Q_ARG(int,canOverwrite))) {
//    g_Application->printMessage("QxrdFileSaverThread::saveData failed");
//  }
//}

//void QxrdFileSaverThread::saveData(QString name, QxrdMaskDataPtr image, int canOverwrite)
//{
//  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
//                                 Q_ARG(QString,name),
//                                 Q_ARG(QxrdMaskDataPtr,image),
//                                 Q_ARG(int,canOverwrite))) {
//    g_Application->printMessage("QxrdFileSaverThread::saveData failed");
//  }
//}

//void QxrdFileSaverThread::saveRawData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
//{
//  if (!QMetaObject::invokeMethod(fileSaver(), "saveRawData",
//                                 Q_ARG(QString,name),
//                                 Q_ARG(QxrdInt32ImageDataPtr,image),
//                                 Q_ARG(QxrdMaskDataPtr,overflow),
//                                 Q_ARG(int,canOverwrite))) {
//    g_Application->printMessage("QxrdFileSaverThread::saveRawData failed");
//  }
//}

//void QxrdFileSaverThread::saveRawData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
//{
//  if (!QMetaObject::invokeMethod(fileSaver(), "saveRawData",
//                                 Q_ARG(QString,name),
//                                 Q_ARG(QxrdInt16ImageDataPtr,image),
//                                 Q_ARG(QxrdMaskDataPtr,overflow),
//                                 Q_ARG(int,canOverwrite))) {
//    g_Application->printMessage("QxrdFileSaverThread::saveRawData failed");
//  }
//}

//void QxrdFileSaverThread::saveTextData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite)
//{
//  if (!QMetaObject::invokeMethod(fileSaver(), "saveTextData",
//                                 Q_ARG(QString,name),
//                                 Q_ARG(QxrdDoubleImageDataPtr,image),
//                                 Q_ARG(QxrdMaskDataPtr,overflow),
//                                 Q_ARG(int,canOverwrite))) {
//    g_Application->printMessage("QxrdFileSaverThread::saveTextData failed");
//  }
//}

//void QxrdFileSaverThread::writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data, QString fileName)
//{
//  if (!QMetaObject::invokeMethod(fileSaver(), "writeOutputScan",
//                                 Q_ARG(FILE*, logFile),
//                                 Q_ARG(QxrdIntegratedDataPtr, data),
//                                 Q_ARG(QString, fileName))) {
//    g_Application->printMessage("QxrdFileSaverThread::writeOutputScan failed");
//  }
//}

//void QxrdFileSaverThread::writeOutputScan(QString dir, QxrdIntegratedDataPtr data, QString fileName)
//{
//  if (!QMetaObject::invokeMethod(fileSaver(), "writeOutputScan",
//                                 Q_ARG(QString, dir),
//                                 Q_ARG(QxrdIntegratedDataPtr, data),
//                                 Q_ARG(QString, fileName))) {
//    g_Application->printMessage("QxrdFileSaverThread::writeOutputScan failed");
//  }
//}
