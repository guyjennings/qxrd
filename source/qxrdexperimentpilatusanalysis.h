#ifndef QXRDEXPERIMENTPILATUSANALYSIS_H
#define QXRDEXPERIMENTPILATUSANALYSIS_H

#include "qxrdexperiment.h"

class QxrdExperimentPilatusAnalysis : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentPilatusAnalysis(QxrdExperimentThreadWPtr expthrd, QString path, QxrdApplication* app);

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPILATUSANALYSIS_H
