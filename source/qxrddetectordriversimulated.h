#ifndef QXRDDETECTORDRIVERSIMULATED_H
#define QXRDDETECTORDRIVERSIMULATED_H

#include "qxrddetectordriver.h"
#include "qxrddetectorsettingssimulated-ptr.h"

class QxrdDetectorDriverSimulated : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverSimulated(QString name,
                              QxrdDetectorSettingsSimulatedPtr sim,
                              QxrdExperimentPtr expt,
                              QxrdAcquisitionPtr acq);

private:
  QxrdDetectorSettingsSimulatedWPtr m_Simulated;
};

#endif // QXRDDETECTORDRIVERSIMULATED_H
