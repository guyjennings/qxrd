#include "qxrdacquisitionexecution.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qxrdacqcommon.h"
#include <QTimer>

QxrdAcquisitionExecution::QxrdAcquisitionExecution(QxrdAcqCommonWPtr acq)
  : inherited("acquisitionExecution"),
    m_Acquiring(this, "acquiring", 0, "Is acquiring?"),
    m_Acquisition(acq)
{
#ifndef QT_NO_DEBUG
  printf("Constructing acquisition execution\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecution::QxrdAcquisitionExecution(%p)\n", this);
  }

  m_IdleTimer = QTimerPtr(new QTimer());

  connect(m_IdleTimer.data(), &QTimer::timeout,
          this,               &QxrdAcquisitionExecution::doAcquireIdle);
}

void QxrdAcquisitionExecution::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  startIdling();
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

void QxrdAcquisitionExecution::setAcquisition(QxrdAcqCommonWPtr acq)
{
  m_Acquisition = acq;
}

void QxrdAcquisitionExecution::doAcquire()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    stopIdling();
    acq -> doAcquire();
    startIdling();
  }
}

void QxrdAcquisitionExecution::doAcquireOnce()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    stopIdling();
    acq -> doAcquireOnce();
    startIdling();
  }
}

void QxrdAcquisitionExecution::doAcquireDark()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    stopIdling();
    acq -> doAcquireDark();
    startIdling();
  }
}

void QxrdAcquisitionExecution::doAcquireIdle()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    acq -> doAcquireIdle();
  }
}

void QxrdAcquisitionExecution::startIdling()
{
  set_Acquiring(false);

  m_IdleTimer->start(5000);
}

void QxrdAcquisitionExecution::stopIdling()
{
  set_Acquiring(true);

  m_IdleTimer->stop();
}
