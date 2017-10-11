#ifndef QXRDDETECTORDRIVEREPICSAREA_H
#define QXRDDETECTORDRIVEREPICSAREA_H

#include "qxrdlib_global.h"
#include "qxrddetectordriver.h"
#include "qxrddetectorsettingsepicsarea-ptr.h"

class QXRD_EXPORT QxrdDetectorDriverEpicsArea : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverEpicsArea(QString name,
                              QxrdDetectorSettingsEpicsAreaPtr det,
                              QxrdExperimentPtr expt,
                              QxrdAcquisitionPtr acq);
  virtual ~QxrdDetectorDriverEpicsArea();

public slots:
  virtual bool startDetectorDriver();
  virtual bool stopDetectorDriver();
  virtual bool changeExposureTime(double exposure);
  virtual bool beginAcquisition(double exposure);
  virtual bool endAcquisition();
  virtual bool shutdownAcquisition();
  virtual void beginFrame();

private:
  QxrdDetectorSettingsEpicsAreaWPtr m_EpicsArea;
};

#endif // QXRDDETECTORDRIVEREPICSAREA_H