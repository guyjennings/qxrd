#ifndef QCEPFIXUPDETECTORPANELGAINS_H
#define QCEPFIXUPDETECTORPANELGAINS_H

#include "qceplib_global.h"
#include "qcepobject.h"
#include "qcepimagedata-ptr.h"

class QCEP_EXPORT QcepFixupDetectorPanelGains : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepFixupDetectorPanelGains();

public:
  static QcepDoubleImageDataPtr exec(QcepDoubleImageDataPtr img);
};

#endif // QCEPFIXUPDETECTORPANELGAINS_H
