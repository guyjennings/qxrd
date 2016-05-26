#ifndef QXRDDETECTORDRIVEREPICSAREA_H
#define QXRDDETECTORDRIVEREPICSAREA_H

#include "qxrddetectordriver.h"
#include "qxrddetectorsettingsepicsarea-ptr.h"

class QxrdDetectorDriverEpicsArea : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverEpicsArea(QString name,
                              QxrdDetectorSettingsEpicsAreaPtr det,
                              QxrdExperimentPtr expt,
                              QxrdAcquisitionPtr acq);

private:
  QxrdDetectorSettingsEpicsAreaWPtr m_EpicsArea;
};

#endif // QXRDDETECTORDRIVEREPICSAREA_H
