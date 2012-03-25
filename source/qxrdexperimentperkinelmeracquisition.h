#ifndef QXRDEXPERIMENTPERKINELMERACQUISITION_H
#define QXRDEXPERIMENTPERKINELMERACQUISITION_H

#include "qxrdexperiment.h"

class QxrdExperimentPerkinElmerAcquisition : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentPerkinElmerAcquisition(QString path, QxrdApplication* app);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPERKINELMERACQUISITION_H
