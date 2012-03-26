#include "qxrddetectorthread.h"
#include "qxrdapplication.h"

#ifdef HAVE_PERKIN_ELMER
#include "qxrddetectorperkinelmer.h"
#endif

#ifdef HAVE_PILATUS
#include "qxrddetectorpilatus.h"
#endif

#ifdef HAVE_AREADETECTOR
#include "qxrddetectorepicsarea.h"
#endif

#include "qxrddetectorfilewatcher.h"
#include "qxrddetectorsimulated.h"

QxrdDetectorThread::QxrdDetectorThread(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QxrdThread(),
  m_Experiment(expt),
  m_Acquisition(acq),
  m_Detector()
{
}

QString QxrdDetectorThread::detectorTypeName(int detectorType)
{
  QString res = "unknown";
  switch (detectorType) {
  case NoDetector:
    res = "No Detector";
    break;

  case SimulatedDetector:
    res = "Simulated Detector";
    break;

  case PerkinElmerDetector:
    res = "Perkin Elmer Detector";
    break;

  case PilatusDetector:
    res = "Pilatus Detector";
    break;

  case EpicsAreaDetector:
    res = "Epics Area Detector";
    break;

  case FileWatcherDetector:
    res = "File Watcher";
    break;
  }

  return res;
}

void QxrdDetectorThread::run()
{
  QxrdDetectorPtr p;

  {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt && qcepDebug(DEBUG_THREADS)) {
      expt->printMessage("Starting Detector Thread");
    }

    if (qcepDebug(DEBUG_THREADS)) {
      printf("Starting Detector Thread\n");

    }
    if (expt) {
      switch(expt->get_DetectorType()) {
      case SimulatedDetector:
        p = QxrdDetectorPtr(new QxrdDetectorSimulated(m_Experiment, m_Acquisition));
        break;

#ifdef HAVE_PERKIN_ELMER
      case PerkinElmerDetector:
        p = QxrdDetectorPtr(new QxrdDetectorPerkinElmer(m_Experiment, m_Acquisition));
        break;
#endif

#ifdef HAVE_PILATUS
      case PilatusDetector:
        p = QxrdDetectorPtr(new QxrdDetectorPilatus(m_Experiment, m_Acquisition));
        break;
#endif

#ifdef HAVE_AREADETECTOR
      case EpicsAreaDetector:
        p = QxrdDetectorPtr(new QxrdDetectorEpicsArea(m_Experiment, m_Acquisition));
        break;
#endif

      case FileWatcherDetector:
        p = QxrdDetectorPtr(new QxrdDetectorFileWatcher(m_Experiment, m_Acquisition));
        break;
      }
    }
  }

  if (p == NULL) {
    p = QxrdDetectorPtr(new QxrdDetectorSimulated(m_Experiment, m_Acquisition));
  }

  int rc = -1;

  if (p) {
    p -> initialize();

    m_Detector = p;

    rc = exec();
  }

  {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt && qcepDebug(DEBUG_THREADS)) {
      expt->printMessage(tr("Detector Thread Terminated with rc %1").arg(rc));
    }

    if (qcepDebug(DEBUG_THREADS)) {
      printf("Detector Thread Terminated with rc %d\n", rc);
    }
  }
}

QxrdDetectorPtr QxrdDetectorThread::detector() const
{
  while (isRunning() && m_Detector == NULL) {
    QThread::msleep(50);
  }

  return m_Detector;
}

void QxrdDetectorThread::shutdown()
{
  exit();

  wait();
}

