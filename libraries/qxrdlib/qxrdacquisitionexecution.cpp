#include "qxrdacquisitionexecution.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qxrdacquisition.h"

QxrdAcquisitionExecution::QxrdAcquisitionExecution(QxrdAcquisitionWPtr acq)
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

void QxrdAcquisitionExecution::setAcquisition(QxrdAcquisitionWPtr acq)
{
  m_Acquisition = acq;
}

void QxrdAcquisitionExecution::doAcquire()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq -> doAcquire();
  }
}

void QxrdAcquisitionExecution::doAcquireOnce()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq -> doAcquireOnce();
  }
}

void QxrdAcquisitionExecution::doAcquireDark()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq -> doAcquireDark();
  }
}
