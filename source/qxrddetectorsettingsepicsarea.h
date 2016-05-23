#ifndef QXRDDETECTORSETTINGSEPICSAREA_H
#define QXRDDETECTORSETTINGSEPICSAREA_H

#include "qxrddetectorsettings.h"

class QxrdDetectorSettingsEpicsArea : public QxrdDetectorSettings
{
  Q_OBJECT

public:
  QxrdDetectorSettingsEpicsArea(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);
  virtual ~QxrdDetectorSettingsEpicsArea();

  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);
};

#endif // QXRDDETECTORSETTINGSEPICSAREA_H
