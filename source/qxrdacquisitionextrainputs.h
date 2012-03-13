#ifndef QXRDACQUISITIONEXTRAINPUTS_H
#define QXRDACQUISITIONEXTRAINPUTS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include <QSettings>
#include <QMutex>

class QxrdAcquisitionExtraInputs : public QObject
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionExtraInputs(QxrdSettingsSaverWPtr saver,
                                      QxrdExperimentWPtr    doc,
                                      QxrdAcquisitionWPtr   acq);
  
public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:
  
public slots:
  
public:
  Q_PROPERTY(int extraInputs READ get_ExtraInputs WRITE set_ExtraInputs)
  QCEP_INTEGER_PROPERTY(ExtraInputs)

private:
  mutable QMutex              m_Mutex;
  QxrdExperimentWPtr          m_Experiment;
  QxrdAcquisitionWPtr         m_Acquisition;
};

#endif // QXRDACQUISITIONEXTRAINPUTS_H
