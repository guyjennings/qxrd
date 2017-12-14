#ifndef QXRDPERKINELMERSETTINGS_H
#define QXRDPERKINELMERSETTINGS_H

#include "qxrdlib_global.h"
#include <QMutex>
#include <QDateTime>
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"

//TODO: rename to QxrdPerkinElmerSettings

class QXRD_EXPORT QxrdPerkinElmerSettings : public QxrdDetectorSettings
{
    Q_OBJECT
public:
  Q_INVOKABLE QxrdPerkinElmerSettings(QString name);
  virtual ~QxrdPerkinElmerSettings();

  void configureDetector();

public:
  typedef enum {
    PCI_SubType,
    GBIF_IP_SubType,
    GBIF_MAC_SubType,
    GBIF_Name_SubType,
    GBIF_Scan_SubType,
    DetectorSubTypeCount
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

  static QStringList timingModeNamesPE();

public:
  Q_PROPERTY(int detectorIndex READ get_DetectorIndex WRITE set_DetectorIndex)
  QCEP_INTEGER_PROPERTY(DetectorIndex)

  Q_PROPERTY(int detectorSubType READ get_DetectorSubType WRITE set_DetectorSubType)
  QCEP_INTEGER_PROPERTY(DetectorSubType)

  Q_PROPERTY(QString detectorAddress READ get_DetectorAddress WRITE set_DetectorAddress)
  QCEP_STRING_PROPERTY(DetectorAddress)

  Q_PROPERTY(int detectorGain READ get_DetectorGain WRITE set_DetectorGain)
  QCEP_INTEGER_PROPERTY(DetectorGain)

  Q_PROPERTY(int detectorBinning READ get_DetectorBinning WRITE set_DetectorBinning)
  QCEP_INTEGER_PROPERTY(DetectorBinning)

  Q_PROPERTY(int detectorTiming READ get_DetectorTiming WRITE set_DetectorTiming)
  QCEP_INTEGER_PROPERTY(DetectorTiming)
};

Q_DECLARE_METATYPE(QxrdPerkinElmerSettings*)

#endif // QXRDPERKINELMERSETTINGS_H
