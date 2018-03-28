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
  void saveImageData         (QcepImageDataBasePtr   image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveDoubleData        (QcepDoubleImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveInt32Data         (QcepUInt32ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveInt16Data         (QcepUInt16ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveMaskData          (QcepMaskDataPtr        image,
                              int                    canOverwrite);

  void saveRaw32Data         (QcepUInt32ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveRaw16Data         (QcepUInt16ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveTextData          (QcepDoubleImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void writeOutputScan       (FILE*                  logFile,
                              QcepIntegratedDataPtr  data,
                              QString                fileName = QString());

  void writeOutputScan       (QString                dir,
                              QcepIntegratedDataPtr  data,
                              QString                fileName = QString());

private slots:
  void saveImageDataPrivate  (QcepImageDataBasePtr   image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveDoubleDataPrivate (QcepDoubleImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveMaskDataPrivate   (QcepMaskDataPtr        image,
                              int                    canOverwrite);

  void saveRaw32DataPrivate  (QcepUInt32ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveRaw16DataPrivate  (QcepUInt16ImageDataPtr image,
                              QcepMaskDataPtr        overflow,
                              int                    canOverwrite);

  void saveTextDataPrivate   (QcepDoubleImageDataPtr image,
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

  QString uniqueFileName     (QcepDataObjectPtr      data,
                              bool                   canOverwrite,
                              QString                extension);

  QString uniqueFileName     (QcepDataObjectPtr      data,
                              bool                   canOverwrite);

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
