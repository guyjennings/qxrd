#ifndef QCEPSETDATAVALUERANGECOMMAND_H
#define QCEPSETDATAVALUERANGECOMMAND_H

#include "qcepserializableobject.h"
#include "qcepimagedata-ptr.h"

class QcepSetDataValueRangeCommand : public QcepSerializableObject
{
  Q_OBJECT

public:
  QcepSetDataValueRangeCommand();

public slots:
  void onImageAvailable(QcepDoubleImageDataPtr img);
  QcepDoubleImageDataPtr exec(QcepDoubleImageDataPtr img);

signals:
  void processedImageAvailable(QcepDoubleImageDataPtr res);
};

#endif // QCEPSETDATAVALUERANGECOMMAND_H
