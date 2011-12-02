#ifndef QXRDEXPERIMENTPILATUSANALYSIS_H
#define QXRDEXPERIMENTPILATUSANALYSIS_H

#include "qxrdexperiment.h"

class QxrdExperimentPilatusAnalysis : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentPilatusAnalysis(QString path, QxrdApplication *app, QSettings *settings=0, QObject *parent = 0);

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPILATUSANALYSIS_H
