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
};

Q_DECLARE_METATYPE(QxrdDexelaSettings*)

#endif // QXRDDEXELASETTINGS_H
