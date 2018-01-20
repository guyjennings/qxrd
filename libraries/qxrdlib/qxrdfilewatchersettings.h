#ifndef QXRDFILEWATCHERSETTINGS_H
#define QXRDFILEWATCHERSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrddetectorsettings.h"

class QXRD_EXPORT QxrdFileWatcherSettings : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdFileWatcherSettings(QString name);

  void configureDetector();
signals:
  
public slots:
  
};

Q_DECLARE_METATYPE(QxrdFileWatcherSettings*)

#endif // QXRDFILEWATCHERSETTINGS_H
