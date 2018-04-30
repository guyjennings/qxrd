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

void QxrdAcqDummy::haltAcquisition()
{
}

void QxrdAcqDummy::registerMetaTypes()
{
  qRegisterMetaType<QxrdAcqDummy*>("QxrdAcqDummy*");
}

int QxrdAcqDummy::detectorCount() const
{
  return 0;
}

QxrdDetectorSettingsPtr QxrdAcqDummy::detector(int i) const
{
  return QxrdDetectorSettingsPtr();
}

QxrdSynchronizerWPtr QxrdAcqDummy::synchronizer()
{
  return QxrdSynchronizerWPtr();
}

QxrdSynchronizedAcquisitionPtr QxrdAcqDummy::synchronizedAcquisition() const
{
  return QxrdSynchronizedAcquisitionPtr();
}

void QxrdAcqDummy::setupAcquisition()
{
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
