#ifndef QCEPSETRANGEOFIMAGECOMMAND_H
#define QCEPSETRANGEOFIMAGECOMMAND_H

#include "qcepserializableobject.h"
#include "qcepimagedata-ptr.h"

class QcepSetRangeOfImageCommand : public QcepSerializableObject
{
  Q_OBJECT

public:
  QcepSetRangeOfImageCommand();

public slots:
  void onImageAvailable(QcepDoubleImageDataPtr img);
  QcepDoubleImageDataPtr exec(QcepDoubleImageDataPtr img);

signals:
  void processedImageAvailable(QcepDoubleImageDataPtr res);
};

#endif // QCEPSETRANGEOFIMAGECOMMAND_H
