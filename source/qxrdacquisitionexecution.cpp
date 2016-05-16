#include "qxrdacquisitionexecution.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qxrdacquisitioninterface.h"

QxrdAcquisitionExecution::QxrdAcquisitionExecution(QxrdAcquisitionInterfaceWPtr acq)
  : QcepObject("acquisitionExecution"),
    m_Acquisition(acq)
{
#ifndef QT_NO_DEBUG
  printf("Constructing acquisition execution\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecution::QxrdAcquisitionExecution(%p)\n", this);
  }
}

QxrdAcquisitionExecution::~QxrdAcquisitionExecution()
{
#ifndef QT_NO_DEBUG
  printf("Deleting acquisition execution\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecution::~QxrdAcquisitionExecution(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisitionExecution::~QxrdAcquisitionExecution");
  }
}

void QxrdAcquisitionExecution::setAcquisition(QxrdAcquisitionInterfaceWPtr acq)
{
  m_Acquisition = acq;
}

void QxrdAcquisitionExecution::doAcquire()
{
  QxrdAcquisitionInterfacePtr acq(m_Acquisition);

  if (acq) {
    acq -> doAcquire();
  }
}

void QxrdAcquisitionExecution::doAcquireDark()
{
  QxrdAcquisitionInterfacePtr acq(m_Acquisition);

  if (acq) {
    acq -> doAcquireDark();
  }
}
