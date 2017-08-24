#ifndef QCEPSCANDETECTORPANELGAINS_H
#define QCEPSCANDETECTORPANELGAINS_H

#include "qcepserializableobject.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepimagedata-ptr.h"

class QcepScanDetectorPanelGains : public QcepSerializableObject
{
public:
  QcepScanDetectorPanelGains();

public:
  static QcepDataColumnScanPtr exec(QcepDoubleImageDataPtr img);

  static QcepDoubleImageDataPtr hEdges(QcepDoubleImageDataPtr img);
  static QcepDoubleImageDataPtr vEdges(QcepDoubleImageDataPtr img);
};

#endif // QCEPSCANDETECTORPANELGAINS_H
