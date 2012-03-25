#ifndef QXRDDETECTOR_H
#define QXRDDETECTOR_H

#include <QObject>

#include "qxrdacquisition-ptr.h"
#include "qxrdexperiment-ptr.h"

class QxrdDetector : public QObject
{
  Q_OBJECT
public:
  explicit QxrdDetector(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);
  virtual ~QxrdDetector();

  virtual void initialize() = 0;

signals:

public slots:

protected:
  QxrdExperimentWPtr  m_Experiment;
  QxrdAcquisitionWPtr m_Acquisition;
};

#endif // QXRDDETECTOR_H
