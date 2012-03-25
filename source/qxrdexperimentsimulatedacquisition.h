#ifndef QXRDEXPERIMENTSIMULATEDACQUISITION_H
#define QXRDEXPERIMENTSIMULATEDACQUISITION_H

#include "qxrdexperiment.h"

class QxrdExperimentSimulatedAcquisition : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentSimulatedAcquisition(QString path, QxrdApplication* app);

signals:

public slots:

};

#endif // QXRDEXPERIMENTSIMULATEDACQUISITION_H
