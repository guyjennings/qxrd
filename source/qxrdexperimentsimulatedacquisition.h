#ifndef QXRDEXPERIMENTSIMULATEDACQUISITION_H
#define QXRDEXPERIMENTSIMULATEDACQUISITION_H

#include "qxrdexperiment.h"

class QxrdExperimentSimulatedAcquisition : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentSimulatedAcquisition(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplicationWPtr app);

signals:

public slots:

};

#endif // QXRDEXPERIMENTSIMULATEDACQUISITION_H
