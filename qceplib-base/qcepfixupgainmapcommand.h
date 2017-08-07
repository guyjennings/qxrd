#ifndef QCEPFIXUPGAINMAPCOMMAND_H
#define QCEPFIXUPGAINMAPCOMMAND_H

#include "qcepserializableobject.h"
#include "qcepimagedata-ptr.h"

class QcepFixupGainMapCommand : public QcepSerializableObject
{
  Q_OBJECT

public:
  QcepFixupGainMapCommand();

public slots:
  void onImageAvailable(QcepDoubleImageDataPtr img);
  QcepDoubleImageDataPtr exec(QcepDoubleImageDataPtr img);

signals:
  void processedImageAvailable(QcepDoubleImageDataPtr res);
};

#endif // QCEPFIXUPGAINMAPCOMMAND_H
