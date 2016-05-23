#include "qxrddebug.h"

#include "qxrddetectordriverthread.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"
#include "qxrddetectordriver.h"

#include <stdio.h>

QxrdDetectorDriverThread::QxrdDetectorDriverThread(QxrdDetectorSettingsWPtr det) :
  QxrdThread(QcepObjectWPtr()),
  m_Detector(det),
  m_DetectorDriver()
{
#ifndef QT_NODEBUG
  printf("Detector thread constructed\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorThread::QxrdDetectorThread(%p)\n", this);
  }

//  setObjectName(detectorTypeName(detType));
}

QxrdDetectorDriverThread::~QxrdDetectorDriverThread()
{
#ifndef QT_NODEBUG
  printf("Detector thread destroyed\n");
#endif

  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorThread::~QxrdDetectorThread(%p)\n", this);
  }
}

void QxrdDetectorDriverThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Detector Thread Started\n");
  }

  int rc = exec();

  if (m_DetectorDriver) {
    m_DetectorDriver -> stopDetectorDriver();
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
