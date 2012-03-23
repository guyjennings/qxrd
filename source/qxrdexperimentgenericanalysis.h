#ifndef QXRDEXPERIMENTGENERICANALYSIS_H
#define QXRDEXPERIMENTGENERICANALYSIS_H

#include "qxrdexperiment.h"

class QxrdExperimentGenericAnalysis : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentGenericAnalysis(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplication* app);

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

#endif // QXRDEXPERIMENTGENERICANALYSIS_H
