#ifndef QXRDDETECTORPROXY_H
#define QXRDDETECTORPROXY_H

#include "qcepobject.h"
#include "qxrddetector-ptr.h"
#include "qxrdacquisition-ptr.h"

class QxrdDetectorProxy : public QcepObject
{
  Q_OBJECT

public:
  explicit QxrdDetectorProxy(QxrdDetectorWPtr det, QxrdAcquisitionWPtr acq);

signals:

public slots:
private:
  QxrdDetectorWPtr m_Detector;
};

#endif // QXRDDETECTORPROXY_H
