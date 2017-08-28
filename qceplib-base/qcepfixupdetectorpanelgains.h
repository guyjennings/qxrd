#ifndef QCEPFIXUPDETECTORPANELGAINS_H
#define QCEPFIXUPDETECTORPANELGAINS_H

#include "qceplib_global.h"
#include "qcepserializableobject.h"
#include "qcepimagedata-ptr.h"

class QCEP_EXPORT QcepFixupDetectorPanelGains : public QcepSerializableObject
{
public:
  QcepFixupDetectorPanelGains();

public:
  static QcepDoubleImageDataPtr exec(QcepDoubleImageDataPtr img);
};

#endif // QCEPFIXUPDETECTORPANELGAINS_H
