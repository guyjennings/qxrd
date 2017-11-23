#ifndef QXRDPROCESSOR_H
#define QXRDPROCESSOR_H

#include "qxrdlib_global.h"
#include "qcepdataprocessorbase.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

//TODO: merge QxrdDataProcessor and QxrdDetectorProcessor into QxrdProcessor
//TODO: separate processing steps into sub-objects
class QXRD_EXPORT QxrdProcessor : public QcepDataProcessorBase
{
public:
  QxrdProcessor(QString name);

signals:
  void processedImageAvailable(QcepDoubleImageDataPtr img);
  void maskAvailable          (QcepMaskDataPtr mask);
};

#endif // QXRDPROCESSOR_H
