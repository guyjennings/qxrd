#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qxrdacquisitionscripting.h"

class QxrdExperiment;
class QxrdDataProcessor;
class QxrdWindow;
typedef QWeakPointer<QxrdExperiment> QxrdExperimentWPtr;
typedef QWeakPointer<QxrdDataProcessor> QxrdDataProcessorWPtr;
#include "qxrdallocator.h"
#include "qxrdsettingssaver.h"

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT
public:
  QxrdAcquisitionOperations(DetectorKind detectorKind, QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdDataProcessorWPtr proc, QxrdAllocatorWPtr allocator);

  void setWindow(QxrdWindow *win);

public slots:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

protected:
  QxrdExperimentWPtr    m_Experiment;
  QxrdWindow           *m_Window;
  QxrdAllocatorWPtr     m_Allocator;
  QxrdDataProcessorWPtr m_DataProcessor;
};

#endif // QXRDACQUISITIONOPERATIONS_H
