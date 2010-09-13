/******************************************************************
*
*  $Id: qxrdfilesaver.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDFILESAVER_H
#define QXRDFILESAVER_H

#include "qcepmacros.h"

#include <QObject>
#include <QReadWriteLock>
#include <QAtomicInt>
#include <QWaitCondition>

#include "qxrdforwardtypes.h"
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdacquisition.h"
#include "qxrdintegrateddata.h"

class QxrdFileSaver : public QObject
{
  Q_OBJECT;

public:
  QxrdFileSaver(QxrdAllocatorPtr allocator, QObject *parent=0);
  ~QxrdFileSaver();

  void setProcessor(QxrdDataProcessorPtr proc);
  void setAcquisition(QxrdAcquisitionPtr acq);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public:
  enum {
    NoOverwrite,
    CanOverwrite
  };

public slots:
  void saveData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite);
  void saveData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite);
  void saveData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite);
  void saveData(QString name, QxrdMaskDataPtr image, int canOverwrite);
  void saveRawData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite);
  void saveRawData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite);
  void saveTextData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite);
  void writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data);

private:
  QString uniqueFileName(QString name);
  QxrdDataProcessorPtr processor() const;

private:
  QxrdDataProcessorPtr m_Processor;
  QxrdAllocatorPtr     m_Allocator;
  QxrdAcquisitionPtr   m_Acquisition;

  HEADER_IDENT("$Id: qxrdfilesaver.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdfilesaver.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.5  2010/06/23 21:41:34  jennings
*  Track file saving time in file saver object
*
*  Revision 1.1.2.4  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
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

