#ifndef QXRDEXPERIMENTPILATUSACQUISITION_H
#define QXRDEXPERIMENTPILATUSACQUISITION_H

#include "qxrdexperiment.h"

class QxrdExperimentPilatusAcquisition : public QxrdExperiment
{
    Q_OBJECT
public:
    explicit QxrdExperimentPilatusAcquisition(QString path, QxrdApplication *app, QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPILATUSACQUISITION_H
