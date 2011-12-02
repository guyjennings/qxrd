#ifndef QXRDEXPERIMENTGENERICANALYSIS_H
#define QXRDEXPERIMENTGENERICANALYSIS_H

#include "qxrdexperiment.h"

class QxrdExperimentGenericAnalysis : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentGenericAnalysis(QString path, QxrdApplication *app, QSettings *settings=0, QObject *parent = 0);

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

#endif // QXRDEXPERIMENTGENERICANALYSIS_H
