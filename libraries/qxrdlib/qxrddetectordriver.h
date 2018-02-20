#ifndef QXRDDETECTORDRIVER_H
#define QXRDDETECTORDRIVER_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrddetectorsettings-ptr.h"

class QXRD_EXPORT QxrdDetectorDriver : public QcepObject
{
  Q_OBJECT

public:
  QxrdDetectorDriver(QString name,
                     QxrdDetectorSettingsWPtr det,
                     QxrdExperimentWPtr expt,
                     QxrdAcqCommonWPtr acq);

public slots:
  virtual void startDetectorDriver() = 0;
  virtual void stopDetectorDriver() = 0;
  virtual void changeExposureTime(double exposure) = 0;
  virtual void beginAcquisition(double exposure) = 0;
  virtual void endAcquisition() = 0;
  virtual void shutdownAcquisition() = 0;
  virtual void beginFrame() = 0;

protected:
  QxrdDetectorSettingsWPtr m_Detector;
  QxrdExperimentWPtr       m_Experiment;
  QxrdAcqCommonWPtr        m_Acquisition;
};

#endif // QXRDDETECTORDRIVER_H
