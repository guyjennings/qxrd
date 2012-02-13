#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qxrdacquisitionscripting.h"
#include "qxrdallocator.h"

class QxrdDataProcessor;
class QxrdWindow;
class QxrdExperiment;

typedef QWeakPointer<QxrdExperiment> QxrdExperimentWPtr;
typedef QWeakPointer<QxrdDataProcessor> QxrdDataProcessorWPtr;
//typedef QWeakPointer<QxrdWindow> QxrdWindowWPtr;
typedef QxrdWindow *QxrdWindowWPtr;

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT
public:
  QxrdAcquisitionOperations(DetectorKind detectorKind, QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdDataProcessorWPtr proc, QxrdAllocatorWPtr allocator);

  void setWindow(QxrdWindowWPtr win);

public slots:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

protected:
  QxrdExperimentWPtr    m_Experiment;
  QxrdWindowWPtr        m_Window;
  QxrdAllocatorWPtr     m_Allocator;
  QxrdDataProcessorWPtr m_DataProcessor;
};

#endif // QXRDACQUISITIONOPERATIONS_H
