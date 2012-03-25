#include "qxrddetectorthread.h"
#include "qxrdapplication.h"
#include "qxrddetectorperkinelmer.h"
#include "qxrddetectorpilatus.h"
#include "qxrddetectorsimulated.h"
#include "qxrddetectorfilewatcher.h"

QxrdDetectorThread::QxrdDetectorThread(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq) :
  QxrdThread(),
  m_Experiment(expt),
  m_Acquisition(acq),
  m_Detector()
{
}

QString QxrdDetectorThread::detectorKindName(int detectorKind)
{
  QString res = "unknown";
  switch (detectorKind) {
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
  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting Detector Thread");
  }

  QxrdExperimentPtr expt(m_Experiment);
  QxrdDetectorPtr p;

  if (expt) {
    switch(expt->get_DetectorType()) {
    case SimulatedDetector:
      p = QxrdDetectorPtr(new QxrdDetectorSimulated(m_Experiment, m_Acquisition));
      break;

#ifdef HAVE_PERKIN_ELMER
    case PerkinElmerDetector:
      if (g_PEAvailable) {
        p = QxrdDetectorPtr(new QxrdDetectorPerkinElmer(m_Experiment, m_Acquisition));
      }
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

  if (p == NULL) {
    p = QxrdDetectorPtr(new QxrdDetectorSimulated(m_Experiment, m_Acquisition));
  }

  m_Detector = p;

  int rc = exec();

  if (g_Application && qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("Detector Thread Terminated with rc %1").arg(rc));
  }
}
