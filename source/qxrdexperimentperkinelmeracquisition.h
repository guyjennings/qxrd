#ifndef QXRDEXPERIMENTPERKINELMERACQUISITION_H
#define QXRDEXPERIMENTPERKINELMERACQUISITION_H

#include "qxrdexperiment.h"

class QxrdExperimentPerkinElmerAcquisition : public QxrdExperiment
{
  Q_OBJECT
public:
  explicit QxrdExperimentPerkinElmerAcquisition(QString path, QxrdApplication* app);

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:

public slots:

};

#endif // QXRDEXPERIMENTPERKINELMERACQUISITION_H
