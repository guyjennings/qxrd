#ifndef QXRDEXPERIMENTPERKINELMERANALYSIS_H
#define QXRDEXPERIMENTPERKINELMERANALYSIS_H

#include "qxrdexperiment.h"

class QxrdExperimentPerkinElmerAnalysis : public QxrdExperiment
{
    Q_OBJECT
public:
    explicit QxrdExperimentPerkinElmerAnalysis(QString path, QxrdApplication *app, QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPERKINELMERANALYSIS_H
