#ifndef QXRDDETECTORPROXY_H
#define QXRDDETECTORPROXY_H

#include "qcepobject.h"
#include "qxrddetectorthread-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrdacquisition-ptr.h"

class QxrdDetectorProxy : public QcepObject
{
  Q_OBJECT

public:
  explicit QxrdDetectorProxy(
      QxrdDetectorThreadPtr thr, QxrdDetectorPtr det, QxrdAcquisitionPtr acq);
  explicit QxrdDetectorProxy(int detectorType,    QxrdAcquisitionPtr acq);

signals:

public slots:
  int                   detectorType();
  QString               detectorTypeName();
  QxrdDetectorThreadPtr detectorThread();
  QxrdDetectorPtr       detector();

private:
  QxrdAcquisitionPtr    m_Acquisition;
  QxrdDetectorThreadPtr m_DetectorThread;
  QxrdDetectorPtr       m_Detector;
  int                   m_DetectorType;
};

#endif // QXRDDETECTORPROXY_H
