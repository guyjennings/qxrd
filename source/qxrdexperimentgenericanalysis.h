#ifndef QXRDEXPERIMENTGENERICANALYSIS_H
#define QXRDEXPERIMENTGENERICANALYSIS_H

#include "qxrdexperiment.h"

class QxrdExperimentGenericAnalysis : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentGenericAnalysis(QString path, QxrdApplicationWPtr app);

signals:

public slots:

};

#endif // QXRDEXPERIMENTGENERICANALYSIS_H
