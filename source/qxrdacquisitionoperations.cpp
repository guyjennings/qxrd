#include "qxrdacquisitionoperations.h"
#include "qxrdimagedata.h"
#include "qxrddataprocessor.h"
#include "qxrdexperiment.h"

QxrdAcquisitionOperations::QxrdAcquisitionOperations(QxrdSettingsSaverWPtr saver,
                                                     QxrdExperimentWPtr doc,
                                                     QxrdDataProcessorWPtr proc,
                                                     QxrdAllocatorWPtr allocator) :
  QxrdAcquisitionScripting(saver),
  m_Experiment(doc),
  m_Window(),
  m_Allocator(allocator),
  m_DataProcessor(proc)
{
}

void QxrdAcquisitionOperations::setWindow(QxrdWindow *win)
{
  m_Window = win;
}

void QxrdAcquisitionOperations::printMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdAcquisitionOperations::criticalMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->criticalMessage(msg);
  }
}

void QxrdAcquisitionOperations::statusMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->statusMessage(msg);
  }
}
