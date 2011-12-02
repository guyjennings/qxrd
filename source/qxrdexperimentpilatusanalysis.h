#ifndef QXRDEXPERIMENTPILATUSANALYSIS_H
#define QXRDEXPERIMENTPILATUSANALYSIS_H

#include "qxrdexperiment.h"

class QxrdExperimentPilatusAnalysis : public QxrdExperiment
{
    Q_OBJECT
public:
    explicit QxrdExperimentPilatusAnalysis(QString path, QxrdApplication *app, QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPILATUSANALYSIS_H
