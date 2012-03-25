#ifndef QXRDDETECTORPERKINELMER_H
#define QXRDDETECTORPERKINELMER_H

#include "qcepmacros.h"
#include "qxrddetector.h"

class QxrdDetectorPerkinElmer : public QxrdDetector
{
    Q_OBJECT
public:
    explicit QxrdDetectorPerkinElmer(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);
    
  void initialize();

signals:
    
public slots:
    
};

#endif // QXRDDETECTORPERKINELMER_H
