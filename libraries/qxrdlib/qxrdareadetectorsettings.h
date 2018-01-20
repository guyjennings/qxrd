#ifndef QXRDAREADETECTORSETTINGS_H
#define QXRDAREADETECTORSETTINGS_H

#include "qxrdlib_global.h"
#include "qxrddetectorsettings.h"

class QXRD_EXPORT QxrdAreaDetectorSettings : public QxrdDetectorSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdAreaDetectorSettings(QString name);
  virtual ~QxrdAreaDetectorSettings();

  void configureDetector();
};

Q_DECLARE_METATYPE(QxrdAreaDetectorSettings*)

#endif // QXRDAREADETECTORSETTINGS_H
