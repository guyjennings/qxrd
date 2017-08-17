#ifndef QCEPFIXUPDETECTORPANELGAINS_H
#define QCEPFIXUPDETECTORPANELGAINS_H

#include "qcepserializableobject.h"
#include "qcepimagedata-ptr.h"

class QcepFixupDetectorPanelGains : public QcepSerializableObject
{
public:
  QcepFixupDetectorPanelGains();

public:
  static QcepDoubleImageDataPtr exec(QcepDoubleImageDataPtr img);
};

#endif // QCEPFIXUPDETECTORPANELGAINS_H
