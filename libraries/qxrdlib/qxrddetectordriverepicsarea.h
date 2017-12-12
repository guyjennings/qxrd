#ifndef QXRDDETECTORDRIVEREPICSAREA_H
#define QXRDDETECTORDRIVEREPICSAREA_H

#include "qxrdlib_global.h"
#include "qxrddetectordriver.h"
#include "qxrdareadetectorsettings-ptr.h"

//TODO: move to plugin
class QXRD_EXPORT QxrdDetectorDriverEpicsArea : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverEpicsArea(QString name,
                              QxrdAreaDetectorSettingsWPtr det,
                              QxrdExperimentWPtr expt,
                              QxrdAcquisitionWPtr acq);
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
  QxrdAreaDetectorSettingsWPtr m_EpicsArea;
};

#endif // QXRDDETECTORDRIVEREPICSAREA_H
