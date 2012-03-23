#ifndef QXRDEXPERIMENTPILATUSACQUISITION_H
#define QXRDEXPERIMENTPILATUSACQUISITION_H

#include "qxrdexperiment.h"

class QxrdExperimentPilatusAcquisition : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentPilatusAcquisition(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplication* app);

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPILATUSACQUISITION_H
