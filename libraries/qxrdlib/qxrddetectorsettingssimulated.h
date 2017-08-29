#ifndef QXRDDETECTORSIMULATED_H
#define QXRDDETECTORSIMULATED_H

#include "qcepmacros.h"
#include "qxrddetectorsettings.h"
#include <QTimer>

class QxrdDetectorSettingsSimulated : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  explicit QxrdDetectorSettingsSimulated(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);
  virtual ~QxrdDetectorSettingsSimulated();

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);

  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

};

#endif // QXRDDETECTORSIMULATED_H
