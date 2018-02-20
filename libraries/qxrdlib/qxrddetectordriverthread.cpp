#include "qxrddebug.h"

#include "qxrddetectordriverthread.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"
#include "qxrddetectordriver.h"
//#include "qxrddetectordriverperkinelmer.h"
#include "qxrddetectorplugininterface.h"

#include <stdio.h>

QxrdDetectorDriverThread::QxrdDetectorDriverThread(QxrdDetectorSettingsWPtr det) :
  QxrdThread(QcepObjectWPtr()),
  m_Detector(det),
  m_DetectorDriver()
{
  QxrdDetectorSettingsPtr d(m_Detector);

#ifndef QT_NO_DEBUG
  if (d) {
    printf("Detector driver thread constructed for %s\n", qPrintable(d->get_DetectorTypeName()));
  } else {
    printf("Detector driver thread constructed for NULL\n");
  }
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorDriverThread::QxrdDetectorDriverThread(%p)\n", this);
  }

  if (d) {
    setObjectName(d->get_DetectorTypeName()+"Thread");
  }
}

QxrdDetectorDriverThread::~QxrdDetectorDriverThread()
{
#ifndef QT_NO_DEBUG
  printf("Detector driver thread destroyed\n");
#endif

  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorDriverThread::~QxrdDetectorDriverThread(%p)\n", this);
  }
}

void QxrdDetectorDriverThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Detector Thread Started\n");
  }

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det) {
    QString name = det->get_DetectorName();

    QxrdExperimentPtr expt(det->experiment());
    QxrdAcqCommonPtr acq(det->acquisition());

    m_DetectorDriver = det->createDetector(name, det, expt, acq);

    if (m_DetectorDriver) {
      m_DetectorDriver -> initialize(sharedFromThis());

      if (det->get_Enabled()) {
        m_DetectorDriver -> startDetectorDriver();
      }
    }
  }

  det = QxrdDetectorSettingsPtr();

  int rc = exec();

  if (m_DetectorDriver) {
    m_DetectorDriver -> stopDetectorDriver();
    m_DetectorDriver = QxrdDetectorDriverPtr();
  }

  m_Detector = QxrdDetectorSettingsPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Detector Thread Terminated with rc %d\n", rc);
  }
}

QxrdDetectorDriverWPtr QxrdDetectorDriverThread::detectorDriver() const
{
//  while (isRunning()) {
//    if (m_Detector) return m_Detector;

//    QThread::msleep(50);
//  }

  return m_DetectorDriver;
}

void QxrdDetectorDriverThread::shutdown()
{
  exit();

  wait();
}

void QxrdDetectorDriverThread::startDetectorDriver()
{
  if (m_DetectorDriver) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), &QxrdDetectorDriver::startDetectorDriver, Qt::QueuedConnection)
    )
#else
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), "startDetectorDriver", Qt::QueuedConnection)
    )
#endif
  }
}

void QxrdDetectorDriverThread::stopDetectorDriver()
{
  if (m_DetectorDriver) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), &QxrdDetectorDriver::stopDetectorDriver, Qt::QueuedConnection)
    )
#else
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), "stopDetectorDriver", Qt::QueuedConnection)
    )
#endif
  }
}

void QxrdDetectorDriverThread::changeExposureTime(double expos)
{
  if (m_DetectorDriver) {
    QxrdDetectorDriver *d = m_DetectorDriver.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
      QMetaObject::invokeMethod(d, [=]() { d->changeExposureTime(expos); }, Qt::QueuedConnection)
    )
#else
    INVOKE_CHECK(
      QMetaObject::invokeMethod(d, "changeExposureTime", Q_ARG(double, expos), Qt::QueuedConnection)
    )
#endif
  }
}

void QxrdDetectorDriverThread::beginAcquisition(double expos)
{
  if (m_DetectorDriver) {
    QxrdDetectorDriver *d = m_DetectorDriver.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
      QMetaObject::invokeMethod(d, [=]() { d->beginAcquisition(expos); }, Qt::QueuedConnection)
    )
#else
    INVOKE_CHECK(
      QMetaObject::invokeMethod(d, "beginAcquisition", Qt::QueuedConnection)
    )
#endif
  }
}

void QxrdDetectorDriverThread::endAcquisition()
{
  if (m_DetectorDriver) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), &QxrdDetectorDriver::endAcquisition, Qt::QueuedConnection)
    )
#else
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), "endAcquisition", Qt::QueuedConnection)
    )
#endif
  }
}

void QxrdDetectorDriverThread::beginFrame()
{
  if (m_DetectorDriver) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), &QxrdDetectorDriver::beginFrame, Qt::QueuedConnection)
    )
#else
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), "beginFrame", Qt::QueuedConnection)
    )
#endif
  }
}

void QxrdDetectorDriverThread::shutdownAcquisition()
{
  if (m_DetectorDriver) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), &QxrdDetectorDriver::shutdownAcquisition, Qt::QueuedConnection)
    )
#else
    INVOKE_CHECK(
      QMetaObject::invokeMethod(m_DetectorDriver.data(), "shutdownAcquisition", Qt::QueuedConnection)
    )
#endif
  }
}

QxrdDetectorDriverThreadPtr QxrdDetectorDriverThread::newDetectorDriverThread(QxrdDetectorSettingsWPtr det)
{
  QxrdDetectorDriverThreadPtr thrd(new QxrdDetectorDriverThread(det));

  return thrd;
}
