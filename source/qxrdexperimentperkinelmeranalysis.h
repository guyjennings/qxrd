#ifndef QXRDEXPERIMENTPERKINELMERANALYSIS_H
#define QXRDEXPERIMENTPERKINELMERANALYSIS_H

#include "qxrdexperiment.h"

class QxrdExperimentPerkinElmerAnalysis : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentPerkinElmerAnalysis(QString path, QxrdApplicationWPtr app);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPERKINELMERANALYSIS_H
