#include "qxrddebug.h"
#include "qxrddetectorthread.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"

#ifdef HAVE_PERKIN_ELMER
#include "qxrddetectorperkinelmer.h"
#endif

//#ifdef HAVE_PILATUS
#include "qxrddetectorpilatus.h"
//#endif

#ifdef HAVE_AREADETECTOR
#include "qxrddetectorepicsarea.h"
#endif

#include "qxrddetectorfilewatcher.h"
#include "qxrddetectorsimulated.h"

#include <stdio.h>

QxrdDetectorThread::QxrdDetectorThread(QcepSettingsSaverWPtr saver,
                                       QxrdExperimentWPtr    expt,
                                       QxrdAcquisitionWPtr   acq,
                                       int                   detType,
                                       QcepObject           *parent) :
  QxrdThread(),
  m_Saver(saver),
  m_Experiment(expt),
  m_Acquisition(acq),
  m_Detector(),
  m_DetectorType(detType),
  m_Parent(parent)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorThread::QxrdDetectorThread(%p)\n", this);
  }
}

QxrdDetectorThread::~QxrdDetectorThread()
{
  if (qcepDebug(DEBUG_APP)) {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt) {
      expt->printMessage("QxrdDetectorThread::~QxrdDetectorThread");
    }
  }

  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorThread::~QxrdDetectorThread(%p)\n", this);
  }
}

int QxrdDetectorThread::detectorTypeCount()
{
  return 6;
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

QStringList QxrdDetectorThread::detectorTypeNames()
{
  QStringList res;

  res.append(detectorTypeName(NoDetector));
  res.append(detectorTypeName(SimulatedDetector));
  res.append(detectorTypeName(PerkinElmerDetector));
  res.append(detectorTypeName(PilatusDetector));
  res.append(detectorTypeName(EpicsAreaDetector));
  res.append(detectorTypeName(FileWatcherDetector));

  return res;
}

QString QxrdDetectorThread::detectorSubTypeName(int detectorSubType)
{
  QString res = "unknown";

  switch (detectorSubType) {
  case PCI_SubType:
    res = "PCI/PCIe Interface";
    break;

  case GBIF_IP_SubType:
    res = "Gigabit Ethernet by IP Address";
    break;

  case GBIF_MAC_SubType:
    res = "Gigabit Ethernet by MAC Address";
    break;

  case GBIF_Name_SubType:
    res = "Gigabit Ethernet by device name";
    break;

  case GBIF_Scan_SubType:
    res = "Gigabit Ethernet by Auto Scan";
    break;
  }

  return res;
}

QStringList QxrdDetectorThread::detectorSubTypeNames()
{
  QStringList res;

  res.append(detectorSubTypeName(PCI_SubType));
  res.append(detectorSubTypeName(GBIF_IP_SubType));
  res.append(detectorSubTypeName(GBIF_MAC_SubType));
  res.append(detectorSubTypeName(GBIF_Name_SubType));
  res.append(detectorSubTypeName(GBIF_Scan_SubType));

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
      switch(m_DetectorType) {
      case SimulatedDetector:
        setObjectName("simulatedDetector");
        p = QxrdDetectorPtr(new QxrdDetectorSimulated(m_Saver, m_Experiment, m_Acquisition, m_Parent));
        break;

#ifdef HAVE_PERKIN_ELMER
      case PerkinElmerDetector:
        setObjectName("perkinElmerDetector");
        p = QxrdDetectorPtr(new QxrdDetectorPerkinElmer(m_Saver, m_Experiment, m_Acquisition, m_Parent));
        break;
#endif

//#ifdef HAVE_PILATUS
      case PilatusDetector:
        setObjectName("pilatusDetector");
        p = QxrdDetectorPtr(new QxrdDetectorPilatus(m_Saver, m_Experiment, m_Acquisition, m_Parent));
        break;
//#endif

#ifdef HAVE_AREADETECTOR
      case EpicsAreaDetector:
        setObjectName("epicsAreaDetector");
        p = QxrdDetectorPtr(new QxrdDetectorEpicsArea(m_Saver, m_Experiment, m_Acquisition, m_Parent));
        break;
#endif

      case FileWatcherDetector:
        setObjectName("fileWatcherDetector");
        p = QxrdDetectorPtr(new QxrdDetectorFileWatcher(m_Saver, m_Experiment, m_Acquisition, m_Parent));
        break;
      }
    }
  }

  if (p == NULL) {
    p = QxrdDetectorPtr(new QxrdDetectorSimulated(m_Saver, m_Experiment, m_Acquisition, m_Parent));
  }

  int rc = -1;

  if (p) {
    p -> initialize();

    m_Mutex.lock();
    m_Detector = p;
    m_Mutex.unlock();

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
  while (isRunning()) {
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_Detector) return m_Detector;
    }

    QThread::msleep(50);
  }

  return QxrdDetectorPtr();
}

void QxrdDetectorThread::shutdown()
{
  exit();

  wait();
}

