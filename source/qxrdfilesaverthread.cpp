/******************************************************************
*
*  $Id: qxrdfilesaverthread.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdfilesaverthread.h"

#include "qxrdfilesaver.h"
#include "qxrdacquisition.h"
#include "qxrdintegrateddata.h"

QxrdFileSaverThread::QxrdFileSaverThread(QxrdAllocatorPtr allocator)
  : QThread(),
    m_FileSaver(NULL),
    m_Allocator(NULL),
    SOURCE_IDENT("$Id: qxrdfilesaverthread.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
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

  connect(m_FileSaver, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));
  connect(m_FileSaver, SIGNAL(statusMessage(QString)), this, SIGNAL(statusMessage(QString)));
  connect(m_FileSaver, SIGNAL(criticalMessage(QString)), this, SIGNAL(criticalMessage(QString)));

  int rc = exec();

  printf("File saver thread terminated with rc %d\n", rc);
}

void QxrdFileSaverThread::shutdown()
{
  exit();

  wait();
}

QxrdFileSaverPtr QxrdFileSaverThread::fileSaver() const
{
  while (m_FileSaver == NULL) {
    QThread::msleep(500);
  }

  return m_FileSaver;
}

void QxrdFileSaverThread::setProcessor(QxrdDataProcessorPtr proc)
{
  fileSaver() -> setProcessor(proc);
}

void QxrdFileSaverThread::setAcquisition(QxrdAcquisitionPtr acq)
{
  fileSaver() -> setAcquisition(acq);
}

void QxrdFileSaverThread::saveData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
                                 Q_ARG(QString,name), Q_ARG(QxrdDoubleImageDataPtr,image), Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveData failed\n");
  }
}

void QxrdFileSaverThread::saveData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
                                 Q_ARG(QString,name), Q_ARG(QxrdInt32ImageDataPtr,image), Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveData failed\n");
  }
}

void QxrdFileSaverThread::saveData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
                                 Q_ARG(QString,name), Q_ARG(QxrdInt16ImageDataPtr,image), Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveData failed\n");
  }
}

void QxrdFileSaverThread::saveData(QString name, QxrdMaskDataPtr image, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveData",
                                 Q_ARG(QString,name), Q_ARG(QxrdMaskDataPtr,image), Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveData failed\n");
  }
}

void QxrdFileSaverThread::saveRawData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveRawData",
                                 Q_ARG(QString,name), Q_ARG(QxrdInt32ImageDataPtr,image), Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveRawData failed\n");
  }
}

void QxrdFileSaverThread::saveRawData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveRawData",
                                 Q_ARG(QString,name), Q_ARG(QxrdInt16ImageDataPtr,image), Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveRawData failed\n");
  }
}

void QxrdFileSaverThread::saveTextData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "saveTextData",
                                 Q_ARG(QString,name), Q_ARG(QxrdDoubleImageDataPtr,image), Q_ARG(int,canOverwrite))) {
    printf("QxrdFileSaverThread::saveTextData failed\n");
  }
}

void QxrdFileSaverThread::writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data)
{
  if (!QMetaObject::invokeMethod(fileSaver(), "writeOutputScan",
                                 Q_ARG(FILE*, logFile), Q_ARG(QxrdIntegratedDataPtr, data))) {
    printf("QxrdFileSaverThread::writeOutputScan failed\n");
  }
}

/******************************************************************
*
*  $Log: qxrdfilesaverthread.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.7  2010/06/23 21:41:34  jennings
*  Track file saving time in file saver object
*
*  Revision 1.1.2.6  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
*
*  Revision 1.1.2.5  2010/06/11 21:23:30  jennings
*  When calling integrator, pass data, dark and mask images explicitly.
*
*  Revision 1.1.2.4  2010/06/09 19:44:21  jennings
*  Print out termination messages when threads quit
*
*  Revision 1.1.2.3  2010/06/09 19:21:03  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*  Moved file saving code to QxrdFileSaver, accessed through QxrdFileSaverThread
*
*  Revision 1.1.2.2  2010/05/24 21:02:38  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.1  2010/05/20 20:15:54  jennings
*  Initial files for QxrdAllocator and QxrdFileSaver threads
*
*
*******************************************************************/
