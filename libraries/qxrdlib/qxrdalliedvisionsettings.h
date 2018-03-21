#ifndef QXRDALLIEDVISIONSETTINGS_H
#define QXRDALLIEDVISIONSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"
#include <QTimer>

class QXRD_EXPORT QxrdAlliedVisionSettings : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdAlliedVisionSettings(QString name);
  virtual ~QxrdAlliedVisionSettings();

  void configureDetector();
};

Q_DECLARE_METATYPE(QxrdAlliedVisionSettings*)

#endif // QXRDALLIEDVISIONSETTINGS_H
