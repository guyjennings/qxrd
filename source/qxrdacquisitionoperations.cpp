#include "qxrdacquisitionoperations.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"
#include "qxrdexperiment.h"

QxrdAcquisitionOperations::QxrdAcquisitionOperations(DetectorKind detectorKind,
                                                     QxrdSettingsSaverPtr saver,
                                                     QxrdExperimentPtr doc,
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

