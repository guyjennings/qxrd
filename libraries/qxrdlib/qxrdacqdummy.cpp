#include "qxrdacqdummy.h"
#include "qxrddebug.h"

QxrdAcqDummy::QxrdAcqDummy(QString name) :
  QxrdAcqCommon(name)
{

}

QxrdAcqDummy::~QxrdAcqDummy()
{
}

void QxrdAcqDummy::initialize(QcepObjectWPtr parent)
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

int QxrdAcqDummy::detectorCount() const
{
  return 0;
}

QxrdDetectorSettingsPtr QxrdAcqDummy::detector(int i) const
{
  return QxrdDetectorSettingsPtr();
}

void QxrdAcqDummy::setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin)
{
}

QxrdNIDAQPluginInterface *QxrdAcqDummy::nidaqPlugin() const
{
  return NULL;
}

QxrdSynchronizedAcquisitionPtr QxrdAcqDummy::synchronizedAcquisition() const
{
  return QxrdSynchronizedAcquisitionPtr();
}

QxrdAcquisitionExtraInputsPtr QxrdAcqDummy::acquisitionExtraInputs() const
{
  return QxrdAcquisitionExtraInputsPtr();
}

void QxrdAcqDummy::acquire()
{
}

void QxrdAcqDummy::acquireOnce()
{
}

void QxrdAcqDummy::acquireDark()
{
}

void QxrdAcqDummy::cancel()
{
}

void QxrdAcqDummy::trigger()
{
}

int QxrdAcqDummy::acquisitionStatus(double time)
{
  return 1;
}

void QxrdAcqDummy::clearDropped()
{
}

void QxrdAcqDummy::configureDetector(int i)
{
}

void QxrdAcqDummy::openDetectorControlWindow(int i)
{
}

//TODO: move into QcepObject?
void QxrdAcqDummy::openWindows()
{
}

void QxrdAcqDummy::closeWindows()
{
}

void QxrdAcqDummy::doAcquire()
{
}

void QxrdAcqDummy::doAcquireIdle()
{
}

void QxrdAcqDummy::doAcquireOnce()
{
}

void QxrdAcqDummy::doAcquireDark()
{
}

QxrdDetectorSettingsPtr QxrdAcqDummy::newDetector(int detType)
{
  return QxrdDetectorSettingsPtr();
}

void QxrdAcqDummy::appendDetector(QxrdDetectorSettingsPtr det)
{
}

void QxrdAcqDummy::clearDetectors()
{
}
