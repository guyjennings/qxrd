#include "qxrdacquisitionprocessor.h"

QxrdAcquisitionProcessor::QxrdAcquisitionProcessor(
    QcepSettingsSaverWPtr saver,
    QxrdExperimentWPtr    doc,
    QxrdDetectorWPtr      det,
    QcepAllocatorWPtr     alloc)
  : QcepObject("acquisitionProcessor", NULL),
    m_Saver(saver),
    m_Experiment(doc),
    m_Detector(det),
    m_Allocator(alloc)
{

}

