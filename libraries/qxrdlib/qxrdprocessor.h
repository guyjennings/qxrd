#ifndef QXRDPROCESSOR_H
#define QXRDPROCESSOR_H

#include "qxrdlib_global.h"
#include "qcepdataprocessorbase.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QXRD_EXPORT QxrdProcessor : public QcepDataProcessorBase
{
public:
  QxrdProcessor(QString name);

signals:
  void processedImageAvailable(QcepDoubleImageDataPtr img);
  void maskAvailable          (QcepMaskDataPtr mask);
};

#endif // QXRDPROCESSOR_H
