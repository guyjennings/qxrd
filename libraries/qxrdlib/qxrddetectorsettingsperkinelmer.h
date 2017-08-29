#ifndef QXRDDETECTORSETTINGSPERKINELMER_H
#define QXRDDETECTORSETTINGSPERKINELMER_H

#include <QMutex>
#include <QDateTime>
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"

class QxrdDetectorSettingsPerkinElmer : public QxrdDetectorSettings
{
    Q_OBJECT
public:
  explicit QxrdDetectorSettingsPerkinElmer(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);
  virtual ~QxrdDetectorSettingsPerkinElmer();

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

public:
  typedef enum {
    PCI_SubType,
    GBIF_IP_SubType,
    GBIF_MAC_SubType,
    GBIF_Name_SubType,
    GBIF_Scan_SubType
  } PEDetectorSubType;

  typedef enum {
    Binning1x1,
    Binning2x2Average,
    Binning2x2Summed,
    BinningModeCount
  } PEBinningMode;

  static QString detectorSubTypeNamePE(int detectorSubType);
  static QStringList detectorSubTypeNamesPE();

  static QStringList gainModeNamesPE();

  static QString     binningModeNamePE(int binningMode);
  static QStringList binningModeNamesPE();

public:
  Q_PROPERTY(int detectorNumber READ get_DetectorNumber WRITE set_DetectorNumber)
  QCEP_INTEGER_PROPERTY(DetectorNumber)

  Q_PROPERTY(int detectorSubType READ get_DetectorSubType WRITE set_DetectorSubType)
  QCEP_INTEGER_PROPERTY(DetectorSubType)

  Q_PROPERTY(QString detectorAddress READ get_DetectorAddress WRITE set_DetectorAddress)
  QCEP_STRING_PROPERTY(DetectorAddress)

  Q_PROPERTY(int detectorGain READ get_DetectorGain WRITE set_DetectorGain)
  QCEP_INTEGER_PROPERTY(DetectorGain)

  Q_PROPERTY(int detectorBinning READ get_DetectorBinning WRITE set_DetectorBinning)
  QCEP_INTEGER_PROPERTY(DetectorBinning)
};

#endif // QXRDDETECTORSETTINGSPERKINELMER_H
