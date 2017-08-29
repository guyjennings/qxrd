#ifndef QXRDFILESAVER_H
#define QXRDFILESAVER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qcepobject.h"
#include <QReadWriteLock>
#include <QAtomicInt>
#include <QWaitCondition>

#include "qcepproperty.h"

#include "qcepallocator-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata.h"
#include "qcepmaskdata-ptr.h"
#include "qcepintegrateddata.h"
#include "qcepintegrateddata-ptr.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdFileSaver : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdFileSaver();
  ~QxrdFileSaver();

  void setProcessor(QxrdDataProcessorWPtr proc);
  void setAcquisition(QxrdAcquisitionWPtr acq);
  void setExperiment(QxrdExperimentWPtr expt);

public:
  enum {
    NoOverwrite,
    CanOverwrite
  };

public slots:
  void saveImageData(QString name, QcepImageDataBasePtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveDoubleData(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveInt32Data(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveInt16Data(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveMaskData(QString name, QcepMaskDataPtr image, int canOverwrite);
  void saveRaw32Data(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveRaw16Data(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveTextData(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void writeOutputScan(FILE* logFile, QcepIntegratedDataPtr data, QString fileName = QString());
  void writeOutputScan(QString dir, QcepIntegratedDataPtr data, QString fileName = QString());

private slots:
  void saveImageDataPrivate(QString name, QcepImageDataBasePtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveDoubleDataPrivate(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveMaskDataPrivate(QString name, QcepMaskDataPtr image, int canOverwrite);
  void saveRaw32DataPrivate(QString name, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveRaw16DataPrivate(QString name, QcepUInt16ImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void saveTextDataPrivate(QString name, QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow, int canOverwrite);
  void writeOutputScanPrivate(FILE* logFile, QcepIntegratedDataPtr data, QString fileName = QString());
  void writeOutputScanPrivate(QString dir, QcepIntegratedDataPtr data, QString fileName = QString());

private:
  void mkPath(QString filePath);
  QString uniqueFileName(QString name);
  QxrdAcquisitionWPtr acquisition() const;
  QxrdExperimentWPtr experiment() const;
  void saveOverflowData(QString name, QcepMaskDataPtr overflow);

  void incBacklog();
  void decBacklog();

private:
  QxrdExperimentWPtr    m_Experiment;
  QxrdDataProcessorWPtr m_Processor;
  QxrdAcquisitionWPtr   m_Acquisition;
};

Q_DECLARE_METATYPE(QxrdFileSaver*)

typedef QSharedPointer<QxrdFileSaver> QxrdFileSaverPtr;
typedef QWeakPointer<QxrdFileSaver> QxrdFileSaverWPtr;

#endif
