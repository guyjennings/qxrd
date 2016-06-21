#include "qxrdacquisitionparameterpack.h"

void QxrdAcquisitionParameterPack::forceSingle()
{
  m_PreTrigger  = 0;
  m_PostTrigger = 1;
}
