#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include "qxrdacquisitionscripting.h"
#include "qxrdimagequeue.h"

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT;
public:
  QxrdAcquisitionOperations(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator);

  QxrdDoubleImageDataPtr takeNextAcquiredImage();

  void newAcquiredImage(QxrdDoubleImageDataPtr img);

protected:
  QxrdAllocatorPtr       m_Allocator;
  QxrdDataProcessorPtr   m_DataProcessor;
};

#endif // QXRDACQUISITIONOPERATIONS_H
