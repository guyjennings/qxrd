#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qxrdacquisitionscripting.h"
#include "qxrdallocator.h"

class QxrdDataProcessor;
class QxrdWindow;
class QxrdExperiment;

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT
public:
  QxrdAcquisitionOperations(DetectorKind detectorKind, QxrdSettingsSaver *saver, QxrdExperiment *doc, QSharedPointer<QxrdDataProcessor> proc, QxrdAllocatorPtr allocator);

  void setWindow(QxrdWindow *win);

protected:
  QxrdExperiment     *m_Experiment;
  QxrdWindow         *m_Window;
  QxrdAllocatorPtr    m_Allocator;
  QSharedPointer<QxrdDataProcessor> m_DataProcessor;
};

#endif // QXRDACQUISITIONOPERATIONS_H
