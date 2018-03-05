#include "qxrddebug.h"
#include "qxrdperkinelmersettings.h"
#include "qxrdperkinelmersettings-ptr.h"
#include "qxrdapplication.h"
#include "qxrdacqcommon.h"
#include "qcepallocator.h"
#include "qcepmutexlocker.h"
#include "qxrdperkinelmerdialog.h"
#include "qxrdperkinelmerdialog-ptr.h"
#include <QThread>

QxrdPerkinElmerSettings::QxrdPerkinElmerSettings(QString name) :
  QxrdDetectorSettings(name, PerkinElmer),
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
    printf("QxrdPerkinElmerSettings::QxrdPerkinElmerSettings(%p)\n", this);
  }

  if (qcepDebug(DEBUG_PERKINELMER)) {
    printMessage("QxrdPerkinElmerSettings::QxrdPerkinElmerSettings()");
  }

  set_CanHardwareSync(true);
}

QxrdPerkinElmerSettings::~QxrdPerkinElmerSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Perkin Elmer Detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdPerkinElmerSettings::~QxrdDetectorSettingsPerkinElmer(%p)\n", this);
  }

  if (qcepDebug(DEBUG_PERKINELMER)) {
    printMessage("QxrdPerkinElmerSettings::~QxrdDetectorSettingsPerkinElmer()");
  }
}

QString QxrdPerkinElmerSettings::detectorSubTypeNamePE(int detectorSubType)
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

QStringList QxrdPerkinElmerSettings::detectorSubTypeNamesPE()
{
  QStringList res;

  res.append(detectorSubTypeNamePE(PCI_SubType));
  res.append(detectorSubTypeNamePE(GBIF_IP_SubType));
  res.append(detectorSubTypeNamePE(GBIF_MAC_SubType));
  res.append(detectorSubTypeNamePE(GBIF_Name_SubType));
  res.append(detectorSubTypeNamePE(GBIF_Scan_SubType));

  return res;
}

QStringList QxrdPerkinElmerSettings::gainModeNamesPE()
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

QString QxrdPerkinElmerSettings::binningModeNamePE(int binningMode)
{
  QString res = "Unknown Binning";

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

QStringList QxrdPerkinElmerSettings::binningModeNamesPE()
{
  QStringList res;

  for (int i=0; i<BinningModeCount; i++) {
    res.append(binningModeNamePE(i));
  }

  return res;
}

QStringList QxrdPerkinElmerSettings::timingModeNamesPE()
{
  QStringList res;

  res.append("Internal");
  res.append("External");

  return res;
}

void QxrdPerkinElmerSettings::configureDetector()
{
  GUI_THREAD_CHECK;

  QxrdPerkinElmerSettingsPtr myself =
      qSharedPointerDynamicCast<QxrdPerkinElmerSettings>(sharedFromThis());

  QxrdPerkinElmerDialogPtr dlog =
      QxrdPerkinElmerDialogPtr(
        new QxrdPerkinElmerDialog(myself));

  if (dlog) {
    dlog->exec();
  }
}
