#ifndef QXRDACQUISITIONTRIGGER_H
#define QXRDACQUISITIONTRIGGER_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include "qcepproperty.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdobjectnamer.h"

class QxrdAcquisitionTrigger : public QObject
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionTrigger(QxrdSettingsSaverPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);

private:
  QxrdObjectNamer m_ObjectNamer;

public:
  Q_PROPERTY(int triggerSync READ get_TriggerSync WRITE set_TriggerSync)
  QCEP_INTEGER_PROPERTY(TriggerSync)

  enum {
    TriggerSyncNone,
    TriggerSyncSynchronized,
    TriggerSyncAsynchronous
  };

  Q_PROPERTY(int triggerAMode READ get_TriggerAMode WRITE set_TriggerAMode)
  QCEP_INTEGER_PROPERTY(TriggerAMode)

  Q_PROPERTY(int triggerACard READ get_TriggerACard WRITE set_TriggerACard)
  QCEP_INTEGER_PROPERTY(TriggerACard)

  Q_PROPERTY(int triggerAChannel READ get_TriggerAChannel WRITE set_TriggerAChannel)
  QCEP_INTEGER_PROPERTY(TriggerAChannel)

  Q_PROPERTY(double triggerALevel READ get_TriggerALevel WRITE set_TriggerALevel)
  QCEP_DOUBLE_PROPERTY(TriggerALevel)

  Q_PROPERTY(double triggerAHysteresis READ get_TriggerAHysteresis WRITE set_TriggerAHysteresis)
  QCEP_DOUBLE_PROPERTY(TriggerAHysteresis)

  Q_PROPERTY(double triggerAValue READ get_TriggerAValue WRITE set_TriggerAValue STORED false)
  QCEP_DOUBLE_PROPERTY(TriggerAValue)

  Q_PROPERTY(double triggerAPrevValue READ get_TriggerAPrevValue WRITE set_TriggerAPrevValue STORED false)
  QCEP_DOUBLE_PROPERTY(TriggerAPrevValue)

  Q_PROPERTY(bool triggerATriggered READ get_TriggerATriggered WRITE set_TriggerATriggered STORED false)
  QCEP_BOOLEAN_PROPERTY(TriggerATriggered)

  Q_PROPERTY(QString triggerAChannelName READ get_TriggerAChannelName WRITE set_TriggerAChannelName)
  QCEP_STRING_PROPERTY(TriggerAChannelName)

  enum {
    TriggerModeNone,
    TriggerModeEdgePos,
    TriggerModeEdgeNeg,
    TriggerModeLevelPos,
    TriggerModeLevelNeg
  };

  enum {
    TriggerCardNone = -1,
    TriggerChannelNone = -1
  };

  Q_PROPERTY(int triggerBMode READ get_TriggerBMode WRITE set_TriggerBMode)
  QCEP_INTEGER_PROPERTY(TriggerBMode)

  Q_PROPERTY(int triggerBCard READ get_TriggerBCard WRITE set_TriggerBCard)
  QCEP_INTEGER_PROPERTY(TriggerBCard)

  Q_PROPERTY(int triggerBChannel READ get_TriggerBChannel WRITE set_TriggerBChannel)
  QCEP_INTEGER_PROPERTY(TriggerBChannel)

  Q_PROPERTY(double triggerBLevel READ get_TriggerBLevel WRITE set_TriggerBLevel)
  QCEP_DOUBLE_PROPERTY(TriggerBLevel)

  Q_PROPERTY(double triggerBHysteresis READ get_TriggerBHysteresis WRITE set_TriggerBHysteresis)
  QCEP_DOUBLE_PROPERTY(TriggerBHysteresis)

  Q_PROPERTY(double triggerBValue READ get_TriggerBValue WRITE set_TriggerBValue STORED false)
  QCEP_DOUBLE_PROPERTY(TriggerBValue)

  Q_PROPERTY(double triggerBPrevValue READ get_TriggerBPrevValue WRITE set_TriggerBPrevValue STORED false)
  QCEP_DOUBLE_PROPERTY(TriggerBPrevValue)

  Q_PROPERTY(bool triggerBTriggered READ get_TriggerBTriggered WRITE set_TriggerBTriggered STORED false)
  QCEP_BOOLEAN_PROPERTY(TriggerBTriggered)

  Q_PROPERTY(QString triggerBChannelName READ get_TriggerBChannelName WRITE set_TriggerBChannelName)
  QCEP_STRING_PROPERTY(TriggerBChannelName)

signals:

public slots:

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  int checkTriggerA();
  int checkTriggerB();

private:
  void timerEvent(QTimerEvent *);
  int evaluateTrigger(
      int wasTriggered, int mode, double level, double hysteresis, double value, double prevvalue);

private:
  mutable QMutex              m_Mutex;
  QxrdExperimentWPtr          m_Experiment;
  QxrdAcquisitionWPtr         m_Acquisition;
};

#endif // QXRDSYNCHRONIZEDACQUISITION_H
