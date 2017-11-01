#ifndef QXRDDETECTORSETTINGSEPICSAREA_H
#define QXRDDETECTORSETTINGSEPICSAREA_H

#include "qxrdlib_global.h"
#include "qxrddetectorsettings.h"

class QXRD_EXPORT QxrdDetectorSettingsEpicsArea : public QxrdDetectorSettings
{
  Q_OBJECT

public:
  QxrdDetectorSettingsEpicsArea(QxrdApplicationWPtr app, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);
  virtual ~QxrdDetectorSettingsEpicsArea();

  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);
};

#endif // QXRDDETECTORSETTINGSEPICSAREA_H
