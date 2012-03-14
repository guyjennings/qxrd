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

  Q_PROPERTY(QStringList channelNames READ get_ChannelNames WRITE set_ChannelNames)
  QCEP_STRING_LIST_PROPERTY(ChannelNames)

  Q_PROPERTY(double sampleRate READ get_SampleRate WRITE set_SampleRate)
  QCEP_DOUBLE_PROPERTY(SampleRate)

  Q_PROPERTY(double startOffset READ get_StartOffset WRITE set_StartOffset)
  QCEP_DOUBLE_PROPERTY(StartOffset)

  Q_PROPERTY(double endOffset READ get_EndOffset WRITE set_EndOffset)
  QCEP_DOUBLE_PROPERTY(EndOffset)

  Q_PROPERTY(double acquireDelay READ get_AcquireDelay WRITE set_AcquireDelay)
  QCEP_DOUBLE_PROPERTY(AcquireDelay)

private:
  mutable QMutex              m_Mutex;
  QxrdExperimentWPtr          m_Experiment;
  QxrdAcquisitionWPtr         m_Acquisition;
};

#endif // QXRDACQUISITIONEXTRAINPUTS_H
