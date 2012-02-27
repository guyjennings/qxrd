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

class QxrdAcquisitionTrigger : public QObject
{
  Q_OBJECT
public:
  explicit QxrdAcquisitionTrigger(QxrdSettingsSaverPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);

public:
  Q_PROPERTY(int triggerSync READ get_TriggerSync WRITE set_TriggerSync)
  QCEP_INTEGER_PROPERTY(TriggerSync)

  enum {
    TriggerSyncNone,
    TriggerSyncSynchronized,
    TriggerSyncAsynchronous
  };

  Q_PROPERTY(int triggerACard READ get_TriggerACard WRITE set_TriggerACard)
  QCEP_INTEGER_PROPERTY(TriggerACard)

  Q_PROPERTY(int triggerAChannel READ get_TriggerAChannel WRITE set_TriggerAChannel)
  QCEP_INTEGER_PROPERTY(TriggerAChannel)

  Q_PROPERTY(int triggerAMode READ get_TriggerAMode WRITE set_TriggerAMode)
  QCEP_INTEGER_PROPERTY(TriggerAMode)

  Q_PROPERTY(int triggerASlope READ get_TriggerASlope WRITE set_TriggerASlope)
  QCEP_INTEGER_PROPERTY(TriggerASlope)

  Q_PROPERTY(double triggerALevel READ get_TriggerALevel WRITE set_TriggerALevel)
  QCEP_DOUBLE_PROPERTY(TriggerALevel)

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

  enum {
    TriggerSlopePositive,
    TriggerSlopeNegative
  };

  Q_PROPERTY(int triggerBCard READ get_TriggerBCard WRITE set_TriggerBCard)
  QCEP_INTEGER_PROPERTY(TriggerBCard)

  Q_PROPERTY(int triggerBChannel READ get_TriggerBChannel WRITE set_TriggerBChannel)
  QCEP_INTEGER_PROPERTY(TriggerBChannel)

  Q_PROPERTY(int triggerBMode READ get_TriggerBMode WRITE set_TriggerBMode)
  QCEP_INTEGER_PROPERTY(TriggerBMode)

  Q_PROPERTY(int triggerBSlope READ get_TriggerBSlope WRITE set_TriggerBSlope)
  QCEP_INTEGER_PROPERTY(TriggerBSlope)

  Q_PROPERTY(double triggerBLevel READ get_TriggerBLevel WRITE set_TriggerBLevel)
  QCEP_DOUBLE_PROPERTY(TriggerBLevel)

signals:

public slots:

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

private:
  mutable QMutex              m_Mutex;
  QxrdExperimentWPtr          m_Experiment;
  QxrdAcquisitionWPtr         m_Acquisition;
  QxrdNIDAQPluginInterfacePtr m_NIDAQPlugin;
};

#endif // QXRDSYNCHRONIZEDACQUISITION_H
