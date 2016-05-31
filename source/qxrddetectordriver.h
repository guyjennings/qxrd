#ifndef QXRDDETECTORDRIVER_H
#define QXRDDETECTORDRIVER_H

#include "qcepobject.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrddetectorsettings-ptr.h"

class QxrdDetectorDriver : public QcepObject
{
  Q_OBJECT

public:
  QxrdDetectorDriver(QString name,
                     QxrdDetectorSettingsPtr det,
                     QxrdExperimentPtr expt,
                     QxrdAcquisitionPtr acq);

public slots:
  virtual bool startDetectorDriver() = 0;
  virtual bool stopDetectorDriver() = 0;
  virtual bool changeExposureTime(double exposure) = 0;
  virtual bool beginAcquisition(double exposure) = 0;
  virtual bool endAcquisition() = 0;
  virtual bool shutdownAcquisition() = 0;
  virtual void beginFrame() = 0;

protected:
  QxrdDetectorSettingsWPtr m_Detector;
  QxrdExperimentWPtr       m_Experiment;
  QxrdAcquisitionWPtr      m_Acquisition;
};

#endif // QXRDDETECTORDRIVER_H
