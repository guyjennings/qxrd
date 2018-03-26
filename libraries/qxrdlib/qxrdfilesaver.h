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
#include "qxrdprocessor-ptr.h"
#include "qxrdacqcommon-ptr.h"
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

  void setProcessor(QxrdProcessorWPtr proc);
  void setAcquisition(QxrdAcqCommonWPtr acq);
  void setExperiment(QxrdExperimentWPtr expt);

public:
  enum {
    NoOverwrite,
    CanOverwrite
  };

public slots:
  void saveImageData         (QString                dirName,
                              QcepImageDataBasePtr   image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveDoubleData        (QString                dirName,
                              QcepDoubleImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveInt32Data         (QString                dirName,
                              QcepUInt32ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveInt16Data         (QString                dirName,
                              QcepUInt16ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveMaskData          (QString                dirName,
                              QcepMaskDataPtr        image,
                              int                    canOverwrite);

  void saveRaw32Data         (QString                dirName,
                              QcepUInt32ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveRaw16Data         (QString                dirName,
                              QcepUInt16ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveTextData          (QString                dirName,
                              QcepDoubleImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void writeOutputScan       (FILE*                  logFile,
                              QcepIntegratedDataPtr  data,
                              QString                fileName = QString());

  void writeOutputScan       (QString                dir,
                              QcepIntegratedDataPtr  data,
                              QString                fileName = QString());

private slots:
  void saveImageDataPrivate  (QString                dirName,
                              QcepImageDataBasePtr   image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveDoubleDataPrivate (QString                dirName,
                              QcepDoubleImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveMaskDataPrivate   (QString                dirName,
                              QcepMaskDataPtr        image,
                              int                    canOverwrite);

  void saveRaw32DataPrivate  (QString                dirName,
                              QcepUInt32ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveRaw16DataPrivate  (QString                dirName,
                              QcepUInt16ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveTextDataPrivate   (QString                dirName,
                              QcepDoubleImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void writeOutputScanPrivate(FILE*                  logFile,
                              QcepIntegratedDataPtr  data,
                              QString                fileName = QString());

  void writeOutputScanPrivate(QString                dir,
                              QcepIntegratedDataPtr  data,
                              QString                fileName = QString());

private:
  void mkPath(QString filePath);
  QString uniqueFileName     (QString                dirName,
                              QcepDataObjectPtr      data,
                              bool                   canOverwrite);

  QString uniqueFileName     (QString                dirName,
                              QcepDataObjectPtr      data,
                              bool                   canOverwrite,
                              QString                extension);

  QxrdAcqCommonWPtr acquisition() const;
  QxrdExperimentWPtr experiment() const;
  void saveOverflowData      (QString                name,
                              QcepMaskDataPtr        overflow);

  void incBacklog();
  void decBacklog();

private:
  QxrdExperimentWPtr m_Experiment;
  QxrdProcessorWPtr  m_Processor;
  QxrdAcqCommonWPtr  m_Acquisition;
};

Q_DECLARE_METATYPE(QxrdFileSaver*)

#endif
