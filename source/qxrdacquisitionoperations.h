#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qxrdacquisitionscripting.h"
#include "qxrdwindow-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdallocator-ptr.h"
#include "qxrddataprocessor-ptr.h"

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
