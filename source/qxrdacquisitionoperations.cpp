#include "qxrdacquisitionoperations.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"

QxrdAcquisitionOperations::QxrdAcquisitionOperations(DetectorKind detectorKind,
                                                     QxrdSettingsSaver *saver,
                                                     QxrdExperiment *doc,
                                                     QSharedPointer<QxrdDataProcessor> proc,
                                                     QxrdAllocatorPtr allocator) :
  QxrdAcquisitionScripting(detectorKind, saver),
  m_Experiment(doc),
  m_Window(NULL),
  m_Allocator(allocator),
  m_DataProcessor(proc)
{
}

void QxrdAcquisitionOperations::setWindow(QxrdWindow *win)
{
  m_Window = win;
}

