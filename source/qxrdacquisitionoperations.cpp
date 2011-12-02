#include "qxrdacquisitionoperations.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"

QxrdAcquisitionOperations::QxrdAcquisitionOperations(QxrdExperiment *doc,
                                                     QxrdDataProcessor *proc,
                                                     QxrdAllocator *allocator) :
  QxrdAcquisitionScripting(),
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

