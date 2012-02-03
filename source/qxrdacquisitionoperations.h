#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qxrdacquisitionscripting.h"
#include "qxrdallocator.h"

class QxrdDataProcessor;
class QxrdWindow;
class QxrdExperiment;

typedef QSharedPointer<QxrdWindow> QxrdWindowPtr;
typedef QSharedPointer<QxrdExperiment> QxrdExperimentPtr;
typedef QSharedPointer<QxrdDataProcessor> QxrdDataProcessorPtr;

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT
public:
  QxrdAcquisitionOperations(DetectorKind detectorKind, QxrdSettingsSaverPtr saver, QxrdExperimentPtr doc, QSharedPointer<QxrdDataProcessor> proc, QxrdAllocatorPtr allocator);

  void setWindow(QxrdWindow *win);

protected:
  QxrdExperimentPtr    m_Experiment;
  QxrdWindow          *m_Window;
  QxrdAllocatorPtr     m_Allocator;
  QxrdDataProcessorPtr m_DataProcessor;
};

#endif // QXRDACQUISITIONOPERATIONS_H
