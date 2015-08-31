#ifndef QXRDMULTIPLEACQUISITION_H
#define QXRDMULTIPLEACQUISITION_H

#include "qxrdacquisition.h"

class QxrdMultipleAcquisition : public QxrdAcquisition
{
public:
  QxrdMultipleAcquisition(QcepSettingsSaverWPtr saver,
                          QxrdExperimentWPtr doc,
                          QxrdDataProcessorWPtr proc,
                          QcepAllocatorWPtr allocator);

signals:

public slots:
};

#endif // QXRDMULTIPLEACQUISITION_H
