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
};

#endif
