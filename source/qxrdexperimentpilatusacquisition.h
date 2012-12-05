#ifndef QXRDEXPERIMENTPILATUSACQUISITION_H
#define QXRDEXPERIMENTPILATUSACQUISITION_H

#include "qxrdexperiment.h"

class QxrdExperimentPilatusAcquisition : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentPilatusAcquisition(QString path, QxrdApplicationWPtr app);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPILATUSACQUISITION_H
