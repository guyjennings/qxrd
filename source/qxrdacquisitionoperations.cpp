#include "qxrdacquisitionoperations.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"

QxrdAcquisitionOperations::QxrdAcquisitionOperations(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator)
  : QxrdAcquisitionScripting(),
    m_Allocator(allocator),
    m_DataProcessor(proc)
{
}
