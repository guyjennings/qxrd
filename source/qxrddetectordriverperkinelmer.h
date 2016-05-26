#ifndef QXRDDETECTORDRIVERPERKINELMER_H
#define QXRDDETECTORDRIVERPERKINELMER_H

#include "qxrddetectordriver.h"
#include "qxrddetectorsettingsperkinelmer-ptr.h"

class QxrdDetectorDriverPerkinElmer : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverPerkinElmer(QString name,
                                QxrdDetectorSettingsPerkinElmerPtr perkinElmer,
                                QxrdExperimentPtr expt,
                                QxrdAcquisitionPtr acq);

private:
  QxrdDetectorSettingsPerkinElmerWPtr m_PerkinElmer;
};

#endif // QXRDDETECTORDRIVERPERKINELMER_H
