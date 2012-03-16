#ifndef QXRDACQUISITIONEXTRAINPUTSCHANNEL_H
#define QXRDACQUISITIONEXTRAINPUTSCHANNEL_H

#include <QObject>
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisitionextrainputs-ptr.h"

class QxrdAcquisitionExtraInputsChannel : public QObject
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionExtraInputsChannel(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionExtraInputsWPtr xtra);
  
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:
  
public slots:
  
public:
  Q_PROPERTY(bool enabled READ get_Enabled WRITE set_Enabled)
  QCEP_BOOLEAN_PROPERTY(Enabled)

  Q_PROPERTY(QString channel READ get_Channel WRITE set_Channel)
  QCEP_STRING_PROPERTY(Channel)

  Q_PROPERTY(int kind READ get_Kind WRITE set_Kind)
  QCEP_INTEGER_PROPERTY(Kind)

  Q_PROPERTY(int mode READ get_Mode WRITE set_Mode)
  QCEP_INTEGER_PROPERTY(Mode)

  Q_PROPERTY(double min READ get_Min WRITE set_Min)
  QCEP_DOUBLE_PROPERTY(Min)

  Q_PROPERTY(double max READ get_Max WRITE set_Max)
  QCEP_DOUBLE_PROPERTY(Max)

  Q_PROPERTY(double start READ get_Start WRITE set_Start)
  QCEP_DOUBLE_PROPERTY(Start)

  Q_PROPERTY(double end READ get_End WRITE set_End)
  QCEP_DOUBLE_PROPERTY(End)

private:
  mutable QMutex                 m_Mutex;
  QxrdExperimentWPtr             m_Experiment;
  QxrdAcquisitionExtraInputsWPtr m_ExtraInputs;
};

#endif // QXRDACQUISITIONEXTRAINPUTSCHANNEL_H
