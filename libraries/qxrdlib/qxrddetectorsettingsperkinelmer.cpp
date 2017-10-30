#include "qxrddebug.h"
#include "qxrddetectorsettingsperkinelmer.h"
#include "qxrdapplication.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include "qcepmutexlocker.h"
#include "qxrddetectorproxy.h"

QxrdDetectorSettingsPerkinElmer::QxrdDetectorSettingsPerkinElmer(QxrdExperimentWPtr    expt,
                                                 QxrdAcquisitionWPtr   acq,
                                                 int                   detNum) :
  QxrdDetectorSettings(expt, acq, PerkinElmerDetector, detNum),
  m_DetectorNumber (this, "detectorNumber",  0, "Perkin Elmer Detector Number"),
  m_DetectorSubType(this, "detectorSubType", 0, "Perkin Elmer Detector Subtype"),
  m_DetectorAddress(this, "detectorAddress", "", "Perkin Elmer Detector Address"),
  m_DetectorGain   (this, "detectorGain",    0, "Detector Gain"),
  m_DetectorBinning(this, "detectorBinning", 0, "Detector Binning Mode"),
  m_DetectorTiming (this, "detectorTiming",  0, "Detector Timing Source")
{
#ifndef QT_NO_DEBUG
  printf("Constructing Perkin Elmer Detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSettingsPerkinElmer::QxrdDetectorSettingsPerkinElmer(%p)\n", this);
  }

  if (qcepDebug(DEBUG_PERKINELMER)) {
    printMessage("QxrdDetectorSettingsPerkinElmer::QxrdDetectorSettingsPerkinElmer()");
  }

//  ::g_Detector = this;
}

QxrdDetectorSettingsPerkinElmer::~QxrdDetectorSettingsPerkinElmer()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Perkin Elmer Detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSettingsPerkinElmer::~QxrdDetectorSettingsPerkinElmer(%p)\n", this);
  }

  if (qcepDebug(DEBUG_PERKINELMER)) {
    printMessage("QxrdDetectorSettingsPerkinElmer::~QxrdDetectorSettingsPerkinElmer()");
  }
}

void QxrdDetectorSettingsPerkinElmer::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
//  QxrdDetectorSettings::pushDefaultsToProxy(proxy, PerkinElmerDetector);

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::PEDetNumProperty, "detectorNumber", "PE Detector Number", 0);
    proxy->pushProperty(QxrdDetectorProxy::PESubTypeProperty, "detectorSubType", "PE Detector Type", 0);
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,   "detectorAddress", "PE Detector Address", "");
    proxy->pushProperty(QxrdDetectorProxy::PEGainProperty,    "detectorGain", "PE Detector Gain", 0);
    proxy->pushProperty(QxrdDetectorProxy::PEBinningProperty, "detectorBinning", "PE Detector Binning", 0);
    proxy->pushProperty(QxrdDetectorProxy::PETimingProperty, "detectorTiming", "PE Detector Timing", 0);
  }
}

void QxrdDetectorSettingsPerkinElmer::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pushPropertiesToProxy(proxy);

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::PEDetNumProperty, "detectorNumber", "PE Detector Number", get_DetectorNumber());
    proxy->pushProperty(QxrdDetectorProxy::PESubTypeProperty, "detectorSubType", "PE Detector Type", get_DetectorSubType());
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,   "detectorAddress", "PE Detector Address", get_DetectorAddress());
    proxy->pushProperty(QxrdDetectorProxy::PEGainProperty,    "detectorGain", "PE Detector Gain", get_DetectorGain());
    proxy->pushProperty(QxrdDetectorProxy::PEBinningProperty, "detectorBinning", "PE Detector Binning", get_DetectorBinning());
    proxy->pushProperty(QxrdDetectorProxy::PETimingProperty, "detectorTiming", "PE Detector Timing", get_DetectorTiming());
  }
}

void QxrdDetectorSettingsPerkinElmer::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pullPropertiesfromProxy(proxy);

  if (proxy) {
    set_DetectorNumber  (proxy->property("detectorNumber").toInt());
    set_DetectorSubType (proxy->property("detectorSubType").toInt());
    set_DetectorAddress (proxy->property("detectorAddress").toString());
    set_DetectorGain    (proxy->property("detectorGain").toInt());
    set_DetectorBinning (proxy->property("detectorBinning").toInt());
    set_DetectorTiming  (proxy->property("detectorTiming").toInt());
  }
}

QString QxrdDetectorSettingsPerkinElmer::detectorSubTypeNamePE(int detectorSubType)
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

QStringList QxrdDetectorSettingsPerkinElmer::detectorSubTypeNamesPE()
{
  QStringList res;

  res.append(detectorSubTypeNamePE(PCI_SubType));
  res.append(detectorSubTypeNamePE(GBIF_IP_SubType));
  res.append(detectorSubTypeNamePE(GBIF_MAC_SubType));
  res.append(detectorSubTypeNamePE(GBIF_Name_SubType));
  res.append(detectorSubTypeNamePE(GBIF_Scan_SubType));

  return res;
}

QStringList QxrdDetectorSettingsPerkinElmer::gainModeNamesPE()
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

QString QxrdDetectorSettingsPerkinElmer::binningModeNamePE(int binningMode)
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

QStringList QxrdDetectorSettingsPerkinElmer::binningModeNamesPE()
{
  QStringList res;

  for (int i=0; i<BinningModeCount; i++) {
    res.append(binningModeNamePE(i));
  }

  return res;
}

QStringList QxrdDetectorSettingsPerkinElmer::timingModeNamesPE()
{
  QStringList res;

  res.append("Internal");
  res.append("External");

  return res;
}
