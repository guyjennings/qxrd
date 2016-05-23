#include "qxrddebug.h"
#include "qxrddetectorthread.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"

#include "qxrddetectorsettingsperkinelmer.h"
#include "qxrddetectorsettingspilatus.h"
#include "qxrddetectorsettingsepicsarea.h"
#include "qxrddetectorsettingsfilewatcher.h"
#include "qxrddetectorsettingssimulated.h"

#include <stdio.h>

QxrdDetectorThread::QxrdDetectorThread(QxrdExperimentWPtr    expt,
                                       QxrdAcquisitionWPtr   acq,
                                       int                   detType,
                                       int                   detNum) :
  QxrdThread(QcepObjectWPtr()),
  m_Experiment(expt),
  m_Acquisition(acq),
  m_Detector(),
  m_DetectorType(detType),
  m_DetectorNumber(detNum)
{
#ifndef QT_NODEBUG
  printf("Detector thread constructed\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorThread::QxrdDetectorThread(%p)\n", this);
  }

  setObjectName(detectorTypeName(detType));
}

QxrdDetectorThread::~QxrdDetectorThread()
{
#ifndef QT_NODEBUG
  printf("Detector thread destroyed\n");
#endif

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

QString QxrdDetectorThread::detectorSubTypeNamePE(int detectorSubType)
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

QStringList QxrdDetectorThread::detectorSubTypeNamesPE()
{
  QStringList res;

  res.append(detectorSubTypeNamePE(PCI_SubType));
  res.append(detectorSubTypeNamePE(GBIF_IP_SubType));
  res.append(detectorSubTypeNamePE(GBIF_MAC_SubType));
  res.append(detectorSubTypeNamePE(GBIF_Name_SubType));
  res.append(detectorSubTypeNamePE(GBIF_Scan_SubType));

  return res;
}

QStringList QxrdDetectorThread::gainModeNamesPE()
{
  QStringList res;

  res.append("0.25 pF (High)");
  res.append("0.5 pF");
  res.append("1 pF");
  res.append("2 pF");
  res.append("4 pF");
  res.append("8 pF (Low)");

  return res;
}

QString QxrdDetectorThread::binningModeNamePE(int binningMode)
{
  QString res = "Unkown Binning";

  switch (binningMode) {
  case Binning1x1:
    res = "No Binning";
    break;

  case Binning2x2Average:
    res = "2x2 Averaged Binning";
    break;

  case Binning2x2Summed:
    res = "2x2 Summed Binning";
    break;
  }

  return res;
}

QStringList QxrdDetectorThread::binningModeNamesPE()
{
  QStringList res;

  for (int i=0; i<BinningModeCount; i++) {
    res.append(binningModeNamePE(i));
  }

  return res;
}

void QxrdDetectorThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Detector Thread Started\n");
  }

  {
    QxrdDetectorPtr det;

    switch(m_DetectorType) {
    case SimulatedDetector:
      setObjectName("simulatedDetector");
      det = QxrdDetectorPtr(new QxrdDetectorSettingsSimulated(m_Experiment,
                                                      m_Acquisition,
                                                      m_DetectorNumber));
      break;

    case PerkinElmerDetector:
      setObjectName("perkinElmerDetector");
      det = QxrdDetectorPtr(new QxrdDetectorSettingsPerkinElmer(m_Experiment,
                                                        m_Acquisition,
                                                        m_DetectorNumber));
      break;

    case PilatusDetector:
      setObjectName("pilatusDetector");
      det = QxrdDetectorPtr(new QxrdDetectorSettingsPilatus(m_Experiment,
                                                    m_Acquisition,
                                                    m_DetectorNumber));
      break;

    case EpicsAreaDetector:
      setObjectName("epicsAreaDetector");
      det = QxrdDetectorPtr(new QxrdDetectorSettingsEpicsArea(m_Experiment,
                                                      m_Acquisition,
                                                      m_DetectorNumber));
      break;

    case FileWatcherDetector:
      setObjectName("fileWatcherDetector");
      det = QxrdDetectorPtr(new QxrdDetectorSettingsFileWatcher(m_Experiment,
                                                        m_Acquisition,
                                                        m_DetectorNumber));
      break;
    }

    if (det == NULL) {
      setObjectName("simulatedDetector");
      det = QxrdDetectorPtr(new QxrdDetectorSettingsSimulated(m_Experiment,
                                                      m_Acquisition,
                                                      m_DetectorNumber));
    }

    if (det) {
      det -> initialize();
    }

    m_Detector = det;
  }

  int rc = exec();

  if (m_Detector) {
    m_Detector -> stopDetector();
  }

  m_Detector = QxrdDetectorPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Detector Thread Terminated with rc %d\n", rc);
  }
}

QxrdDetectorWPtr QxrdDetectorThread::detector() const
{
  while (isRunning()) {
    if (m_Detector) return m_Detector;

    QThread::msleep(50);
  }

  return QxrdDetectorWPtr();
}

void QxrdDetectorThread::shutdown()
{
  exit();

  wait();
}

void QxrdDetectorThread::pushDefaultsToProxy(int detectorType,
                                             QxrdDetectorProxyPtr proxy)
{
  switch (detectorType) {
  case NoDetector:
  default:
    QxrdDetectorSettings::pushDefaultsToProxy(proxy, QxrdDetectorThread::NoDetector);
    break;

  case SimulatedDetector:
    QxrdDetectorSettingsSimulated::pushDefaultsToProxy(proxy);
    break;

#ifdef HAVE_PERKIN_ELMER
  case PerkinElmerDetector:
    QxrdDetectorSettingsPerkinElmer::pushDefaultsToProxy(proxy);
    break;
#endif

  case PilatusDetector:
    QxrdDetectorSettingsPilatus::pushDefaultsToProxy(proxy);
    break;

#ifdef HAVE_AREADETECTOR
  case EpicsAreaDetector:
    QxrdDetectorSettingsEpicsArea::pushDefaultsToProxy(proxy);
    break;
#endif

  case FileWatcherDetector:
    QxrdDetectorSettingsFileWatcher::pushDefaultsToProxy(proxy);
    break;
  }
}
