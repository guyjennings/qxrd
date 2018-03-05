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
  Q_PROPERTY(double readoutTime READ get_ReadoutTime WRITE set_ReadoutTime STORED false)
  QCEP_DOUBLE_PROPERTY(ReadoutTime)
};

Q_DECLARE_METATYPE(QxrdDexelaSettings*)

#endif // QXRDDEXELASETTINGS_H
