#ifndef QXRDEXPERIMENTSIMULATEDACQUISITION_H
#define QXRDEXPERIMENTSIMULATEDACQUISITION_H

#include "qxrdexperiment.h"

class QxrdExperimentSimulatedAcquisition : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentSimulatedAcquisition(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplication* app);

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

#endif // QXRDEXPERIMENTSIMULATEDACQUISITION_H
