#ifndef QXRDPROCESSOREXECUTION_H
#define QXRDPROCESSOREXECUTION_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdprocessor-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QXRD_EXPORT QxrdProcessorExecution : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdProcessorExecution(QString name);
  virtual ~QxrdProcessorExecution();
  void initialize(QcepObjectWPtr parent);
  static void registerMetaTypes();

  Q_INVOKABLE void haltProcessorExecution();
  Q_INVOKABLE void processAcquiredImage(QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  Q_INVOKABLE void processDarkImage    (QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow);
  Q_INVOKABLE void processIdleImage    (QcepImageDataBasePtr image);

private:
  QcepImageDataBasePtr doDarkSubtraction    (QcepImageDataBasePtr img);
  QcepImageDataBasePtr doBadPixels          (QcepImageDataBasePtr img);
  QcepImageDataBasePtr doGainCorrection     (QcepImageDataBasePtr img);
  QcepDoubleVector     doCalculateROICounts (QcepImageDataBasePtr img);
  void                 doSaveRawImage       (QcepImageDataBasePtr img,
                                             QcepMaskDataPtr ovf);
  void                 doSaveSubtractedImage(QcepImageDataBasePtr img,
                                             QcepMaskDataPtr ovf);
  void                 doSaveDarkImage      (QcepImageDataBasePtr img,
                                             QcepMaskDataPtr ovf);

  void setCommonImageProperties(QcepImageDataBasePtr image);
  void setAcquiredImageProperties(QcepImageDataBasePtr image);
  void setDarkImageProperties(QcepImageDataBasePtr image);
  void setIdleImageProperties(QcepImageDataBasePtr image);
  void setSubtractedImageProperties(QcepImageDataBasePtr image);

public:
  Q_PROPERTY(int processing READ get_Processing WRITE set_Processing STORED false)
  QCEP_INTEGER_PROPERTY(Processing)

private:
  QxrdProcessorWPtr m_Processor;
  QxrdAcqCommonWPtr m_Acquisition;
};

Q_DECLARE_METATYPE(QxrdProcessorExecution*)

#endif // QXRDPROCESSOREXECUTION_H
