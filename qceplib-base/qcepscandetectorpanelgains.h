#ifndef QCEPSCANDETECTORPANELGAINS_H
#define QCEPSCANDETECTORPANELGAINS_H

#include "qceplib_global.h"
#include "qcepserializableobject.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepimagedata-ptr.h"

class QCEP_EXPORT QcepScanDetectorPanelGains : public QcepSerializableObject
{
public:
  QcepScanDetectorPanelGains();

public:
  static QcepDataColumnScanPtr exec(QcepDoubleImageDataPtr img);

  static QcepDoubleImageDataPtr hEdges(QcepDoubleImageDataPtr img);
  static QcepDoubleImageDataPtr vEdges(QcepDoubleImageDataPtr img);
};

#endif // QCEPSCANDETECTORPANELGAINS_H
