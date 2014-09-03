#ifndef QCEPEXPERIMENT_H
#define QCEPEXPERIMENT_H

#include "qcepobject.h"
#include <QDateTime>

class QcepExperiment : public QcepObject
{
  Q_OBJECT
public:
  explicit QcepExperiment(QString name, QObject *parent = 0);

signals:

public slots:

//  virtual void criticalMessage(QString msg) = 0;
//  virtual void statusMessage(QString msg) = 0;
//  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
};

#endif // QCEPEXPERIMENT_H
