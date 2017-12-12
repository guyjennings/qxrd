#include "qxrddebug.h"

#include "qxrddetectordriverthread.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"
#include "qxrddetectordriver.h"
#include "qxrddetectordriverepicsarea.h"
#include "qxrdareadetectorsettings.h"
#include "qxrddetectordriverfilewatcher.h"
#include "qxrdfilewatchersettings.h"
#include "qxrddetectordriverperkinelmer.h"
#include "qxrdperkinelmersettings.h"
#include "qxrddetectordriverdexela.h"
#include "qxrddexelasettings.h"
#include "qxrddetectordriverpilatus.h"
#include "qxrdpilatussettings.h"
#include "qxrddetectordriversimulated.h"
#include "qxrdsimulatedsettings.h"
#include "qxrddetectorplugininterface.h"

#include <stdio.h>

QxrdDetectorDriverThread::QxrdDetectorDriverThread(QxrdDetectorSettingsWPtr det) :
  QxrdThread(QcepObjectWPtr()),
  m_Detector(det),
  m_DetectorDriver()
{
#ifndef QT_NO_DEBUG
  QxrdDetectorSettingsPtr d(m_Detector);

  if (d) {
    printf("Detector driver thread constructed for %s\n", qPrintable(d->get_DetectorTypeName()));
  } else {
    printf("Detector driver thread constructed for NULL\n");
  }
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorDriverThread::QxrdDetectorDriverThread(%p)\n", this);
  }

//  setObjectName(detectorTypeName(detType));
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
    QxrdAcquisitionPtr acq(det->acquisition());

    m_DetectorDriver = det->createDetector(name, det, expt, acq);
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

bool QxrdDetectorDriverThread::startDetectorDriver()
{
  if (m_DetectorDriver) {
    return QMetaObject::invokeMethod(m_DetectorDriver.data(), "startDetectorDriver");
  } else {
    return false;
  }
}

bool QxrdDetectorDriverThread::stopDetectorDriver()
{
  if (m_DetectorDriver) {
    return QMetaObject::invokeMethod(m_DetectorDriver.data(), "stopDetectorDriver");
  } else {
    return false;
  }
}

bool QxrdDetectorDriverThread::changeExposureTime(double expos)
{
  if (m_DetectorDriver) {
    return QMetaObject::invokeMethod(m_DetectorDriver.data(), "changeExposureTime", Q_ARG(double, expos));
  } else {
    return false;
  }
}

bool QxrdDetectorDriverThread::beginAcquisition(double expos)
{
  if (m_DetectorDriver) {
    return QMetaObject::invokeMethod(m_DetectorDriver.data(), "beginAcquisition", Q_ARG(double, expos));
  } else {
    return false;
  }
}

bool QxrdDetectorDriverThread::endAcquisition()
{
  if (m_DetectorDriver) {
    return QMetaObject::invokeMethod(m_DetectorDriver.data(), "endAcquisition");
  } else {
    return false;
  }
}

void QxrdDetectorDriverThread::beginFrame()
{
  if (m_DetectorDriver) {
    QMetaObject::invokeMethod(m_DetectorDriver.data(), "beginFrame");
  }
}

bool QxrdDetectorDriverThread::shutdownAcquisition()
{
  if (m_DetectorDriver) {
    return QMetaObject::invokeMethod(m_DetectorDriver.data(), "shutdownAcquisition");
  } else {
    return false;
  }
}

QxrdDetectorDriverThreadPtr QxrdDetectorDriverThread::newDetectorDriverThread(QxrdDetectorSettingsWPtr det)
{
  QxrdDetectorDriverThreadPtr thrd(new QxrdDetectorDriverThread(det));

  return thrd;
}
