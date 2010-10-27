/******************************************************************
*
*  $Id: qxrdfilesaverthread.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDFILESAVERTHREAD_H
#define QXRDFILESAVERTHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include "qxrdforwardtypes.h"
#include "qxrdfilesaver.h"

class QxrdFileSaverThread : public QThread
{
  Q_OBJECT;

public:
  QxrdFileSaverThread(QxrdAllocatorPtr acq);
  ~QxrdFileSaverThread();

  void shutdown();

  void setProcessor(QxrdDataProcessorPtr proc);
  void setAcquisition(QxrdAcquisitionPtr acq);

  void saveData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite);
  void saveData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite);
  void saveData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite);
  void saveData(QString name, QxrdMaskDataPtr image, int canOverwrite);
  void saveRawData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite);
  void saveRawData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite);
  void saveTextData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite);
  void writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

protected:
  void run();

private:
  QxrdFileSaverPtr fileSaver() const;

private:
  QAtomicPointer<QxrdFileSaver>     m_FileSaver;
  QAtomicPointer<QxrdAllocator>     m_Allocator;
  HEADER_IDENT("$Id: qxrdfilesaverthread.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDFILESAVERTHREAD_H

/******************************************************************
*
*  $Log: qxrdfilesaverthread.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.5  2010/06/23 21:41:34  jennings
*  Track file saving time in file saver object
*
*  Revision 1.1.2.4  2010/06/11 21:23:30  jennings
*  When calling integrator, pass data, dark and mask images explicitly.
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
*
*******************************************************************/
