#include "qxrdacqdummy.h"
#include "qxrddebug.h"

QxrdAcqDummy::QxrdAcqDummy(QString name) :
  QxrdAcqCommon(name)
{

}

QxrdAcqDummy::~QxrdAcqDummy()
{
}

void QxrdAcqDummy::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);
}

QxrdAcqDummyPtr QxrdAcqDummy::newAcquisition()
{
  QxrdAcqDummyPtr acq(new QxrdAcqDummy("acquisition"));

  if (qcepDebug(DEBUG_APP)) {
    acq->printMessage("QxrdAcqDummy::newAcquisition");
  }

  return acq;
}
