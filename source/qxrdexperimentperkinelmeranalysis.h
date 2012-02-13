#ifndef QXRDEXPERIMENTPERKINELMERANALYSIS_H
#define QXRDEXPERIMENTPERKINELMERANALYSIS_H

#include "qxrdexperiment.h"

class QxrdExperimentPerkinElmerAnalysis : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentPerkinElmerAnalysis(QString path, QxrdApplication* app, QSettings *settings=0);

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPERKINELMERANALYSIS_H
