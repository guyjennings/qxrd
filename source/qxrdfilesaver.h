#ifndef QXRDFILESAVER_H
#define QXRDFILESAVER_H

#include "qcepmacros.h"

#include <QObject>
#include <QReadWriteLock>
#include <QAtomicInt>
#include <QWaitCondition>

#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdacquisition.h"
#include "qxrdintegrateddata.h"

class QxrdAllocator;
class QxrdDataProcessor;
class QxrdAcquisition;

class QxrdFileSaver : public QObject
{
  Q_OBJECT

public:
  QxrdFileSaver(QxrdAllocator *allocator, QObject *parent=0);
  ~QxrdFileSaver();

  void setProcessor(QxrdDataProcessor *proc);
  void setAcquisition(QxrdAcquisition *acq);

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
  void writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data, QString fileName);
  void writeOutputScan(QString dir, QxrdIntegratedDataPtr data, QString fileName);

private:
  void mkPath(QString filePath);
  QString uniqueFileName(QString name);
  QxrdDataProcessor *processor() const;
  QxrdAcquisition *acquisition() const;
  void saveOverflowData(QString name, QxrdMaskDataPtr overflow);

private:
  QxrdDataProcessor *m_Processor;
  QxrdAllocator     *m_Allocator;
  QxrdAcquisition   *m_Acquisition;
};

#endif
