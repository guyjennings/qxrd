#include "qxrdsynchronizedacquisition.h"

QxrdSynchronizedAcquisition::QxrdSynchronizedAcquisition(QxrdAcquisition *acq) :
  m_Acquisition(acq),
  QObject(NULL)
{
}
