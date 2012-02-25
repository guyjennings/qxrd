#ifndef QXRDFILESAVER_H
#define QXRDFILESAVER_H

#include "qcepmacros.h"

#include <QObject>
#include <QReadWriteLock>
#include <QAtomicInt>
#include <QWaitCondition>

#include "qcepproperty.h"
#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdintegrateddata.h"
#include "qxrdallocator.h"

class QxrdDataProcessor;
class QxrdAcquisition;
typedef QWeakPointer<QxrdDataProcessor> QxrdDataProcessorWPtr;
typedef QWeakPointer<QxrdAcquisition> QxrdAcquisitionWPtr;

class QxrdFileSaver : public QObject
{
  Q_OBJECT

public:
  QxrdFileSaver(QxrdAllocatorWPtr allocator);
  ~QxrdFileSaver();

  void setProcessor(QxrdDataProcessorWPtr proc);
  void setAcquisition(QxrdAcquisitionWPtr acq);

public:
  enum {
    NoOverwrite,
    CanOverwrite
  };

public slots:
  void saveData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveData(QString name, QxrdMaskDataPtr image, int canOverwrite);
  void saveRawData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveRawData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveTextData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data, QString fileName = QString());
  void writeOutputScan(QString dir, QxrdIntegratedDataPtr data, QString fileName = QString());

private:
  void mkPath(QString filePath);
  QString uniqueFileName(QString name);
  QxrdAcquisitionWPtr acquisition() const;
  void saveOverflowData(QString name, QxrdMaskDataPtr overflow);

private:
  QxrdDataProcessorWPtr m_Processor;
  QxrdAllocatorWPtr     m_Allocator;
  QxrdAcquisitionWPtr   m_Acquisition;
};

typedef QSharedPointer<QxrdFileSaver> QxrdFileSaverPtr;
typedef QWeakPointer<QxrdFileSaver> QxrdFileSaverWPtr;

#endif
