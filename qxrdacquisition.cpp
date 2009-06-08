#include "qxrdacquisition.h"

QxrdAcquisition::QxrdAcquisition(QxrdAcquisitionThread *thread)
  : inherited(thread)
{
  emit printMessage("Enter QxrdAcquisition::QxrdAcquisition\n");
}


