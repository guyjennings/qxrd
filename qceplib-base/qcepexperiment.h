#ifndef QCEPEXPERIMENT_H
#define QCEPEXPERIMENT_H

#include "qcepobject.h"
#include <QDateTime>
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QcepExperiment : public QcepObject
{
  Q_OBJECT
public:
  explicit QcepExperiment(QString path, QString name, QObject *parent = 0);

signals:

public slots:

  QString defaultExperimentDirectory(QString path) const;
  QString defaultExperimentFileName(QString path) const;
  QString defaultExperimentName(QString path) const;

protected:
  QcepSettingsSaverPtr            m_SettingsSaver;

public:
  Q_PROPERTY(int experimentKind READ get_ExperimentKind WRITE set_ExperimentKind)
  QCEP_INTEGER_PROPERTY(ExperimentKind)

  Q_PROPERTY(QString experimentDirectory     READ get_ExperimentDirectory WRITE set_ExperimentDirectory STORED false)
  QCEP_STRING_PROPERTY(ExperimentDirectory)

  Q_PROPERTY(QString experimentFileName     READ get_ExperimentFileName WRITE set_ExperimentFileName STORED false)
  QCEP_STRING_PROPERTY(ExperimentFileName)

  Q_PROPERTY(QString experimentName     READ get_ExperimentName WRITE set_ExperimentName STORED false)
  QCEP_STRING_PROPERTY(ExperimentName)

  Q_PROPERTY(QString experimentDescription     READ get_ExperimentDescription WRITE set_ExperimentDescription)
  QCEP_STRING_PROPERTY(ExperimentDescription)
};

#endif // QCEPEXPERIMENT_H
