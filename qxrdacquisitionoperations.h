#ifndef QXRDACQUISITIONOPERATIONS_H
#define QXRDACQUISITIONOPERATIONS_H

#include "qxrdacquisitionscripting.h"

#include "qxrdimagequeue.h"

class QxrdAcquisitionOperations : public QxrdAcquisitionScripting
{
  Q_OBJECT;
public:
  QxrdAcquisitionOperations(QxrdAcquisitionThread *thread);
  typedef QxrdAcquisitionScripting inherited;

  QxrdImageData *takeNextFreeImage();
  QxrdImageData *takeNextAcquiredImage();

  void returnImageToPool(QxrdImageData *img);
  void newAcquiredImage(QxrdImageData *img);

signals:
  void acquiredImageAvailable();

private:
  QxrdImageQueue         m_FreeImages;
  QxrdImageQueue         m_AcquiredImages;
};

#endif // QXRDACQUISITIONOPERATIONS_H
