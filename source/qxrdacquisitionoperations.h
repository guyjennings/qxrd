#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qxrdacquisitionscripting.h"

class QxrdDataProcessor;
class QxrdAllocator;
class QxrdWindow;

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT
public:
  QxrdAcquisitionOperations(QxrdDataProcessor *proc, QxrdAllocator *allocator);

  void setWindow(QxrdWindow *win);

protected:
  QxrdWindow         *m_Window;
  QxrdAllocator      *m_Allocator;
  QxrdDataProcessor  *m_DataProcessor;
};

#endif // QXRDACQUISITIONOPERATIONS_H
