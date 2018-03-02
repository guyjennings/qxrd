#ifndef QXRDDEXELASETTINGS_H
#define QXRDDEXELASETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"
#include <QTimer>

class QXRD_EXPORT QxrdDexelaSettings : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDexelaSettings(QString name);
  virtual ~QxrdDexelaSettings();

  void configureDetector();

  enum {
    SoftwareSync,
    HardwareSync
  };

public:
  Q_PROPERTY(int detectorIndex READ get_DetectorIndex WRITE set_DetectorIndex)
  QCEP_INTEGER_PROPERTY(DetectorIndex)

  Q_PROPERTY(double exposureFactor READ get_ExposureFactor WRITE set_ExposureFactor)
  QCEP_DOUBLE_PROPERTY(ExposureFactor)

  Q_PROPERTY(bool hardwareSync READ get_HardwareSync WRITE set_HardwareSync)
  QCEP_BOOLEAN_PROPERTY(HardwareSync)

  Q_PROPERTY(double readoutTime READ get_ReadoutTime WRITE set_ReadoutTime STORED false)
  QCEP_DOUBLE_PROPERTY(ReadoutTime)
};

Q_DECLARE_METATYPE(QxrdDexelaSettings*)

#endif // QXRDDEXELASETTINGS_H
