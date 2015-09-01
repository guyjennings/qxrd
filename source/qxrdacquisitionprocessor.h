#ifndef QXRDACQUISITIONPROCESSOR_H
#define QXRDACQUISITIONPROCESSOR_H

#include "qcepmacros.h"
#include "qcepobject.h"

#include "qcepsettingssaver-ptr.h"
#include "qxrdmultipleacquisition-ptr.h"
#include "qxrddetector-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepallocator-ptr.h"

class QxrdAcquisitionProcessor : public QcepObject, public QEnableSharedFromThis<QxrdAcquisitionProcessor>
{
    Q_OBJECT

public:
  QxrdAcquisitionProcessor(QcepSettingsSaverWPtr saver,
                           QxrdExperimentWPtr    doc,
                           QxrdDetectorWPtr      det,
                           QcepAllocatorWPtr     alloc);

signals:

public slots:

public:

private:
  QcepSettingsSaverWPtr m_Saver;

public:
  // Properties...

private:
  QxrdExperimentWPtr    m_Experiment;
  QxrdDetectorWPtr      m_Detector;
  QcepAllocatorWPtr     m_Allocator;
};

#endif // QXRDACQUISITIONPROCESSOR_H
