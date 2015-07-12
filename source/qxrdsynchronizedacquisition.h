#ifndef QXRDSYNCHRONIZEDACQUISITION_H
#define QXRDSYNCHRONIZEDACQUISITION_H

#include "qcepobject.h"
#include <QVector>
#include <QMutex>
#include "qcepproperty.h"
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdacquisition-ptr.h"
#include "qcepsettingssaver-ptr.h"

class QxrdSynchronizedAcquisition : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdSynchronizedAcquisition(QcepSettingsSaverPtr saver, QxrdAcquisition *acq);

public:
  Q_PROPERTY(int syncAcquisitionMode READ get_SyncAcquisitionMode WRITE set_SyncAcquisitionMode)
  QCEP_INTEGER_PROPERTY(SyncAcquisitionMode)

  enum {
    SyncAcquisitionModeNone,
    SyncAcquisitionModeStepped,
    SyncAcquisitionModeContinuous
  };

  Q_PROPERTY(int syncAcquisitionWaveform READ get_SyncAcquisitionWaveform WRITE set_SyncAcquisitionWaveform)
  QCEP_INTEGER_PROPERTY(SyncAcquisitionWaveform)

  enum {
    SyncAcquisitionWaveformSquare,
    SyncAcquisitionWaveformSine,
    SyncAcquisitionWaveformTriangle,
    SyncAcquisitionWaveformSawtooth,
    SyncAcquisitionWaveformBipolarTriangle
  };

  Q_PROPERTY(QString syncAcquisitionOutputDevice READ get_SyncAcquisitionOutputDevice WRITE set_SyncAcquisitionOutputDevice)
  QCEP_STRING_PROPERTY(SyncAcquisitionOutputDevice)

  Q_PROPERTY(QString syncAcquisitionOutputChannel READ get_SyncAcquisitionOutputChannel WRITE set_SyncAcquisitionOutputChannel)
  QCEP_STRING_PROPERTY(SyncAcquisitionOutputChannel)

//  Q_PROPERTY(int syncAcquisitionFlagChannel READ get_SyncAcquisitionFlagChannel WRITE set_SyncAcquisitionFlagChannel)
//  QCEP_INTEGER_PROPERTY(SyncAcquisitionFlagChannel)

  Q_PROPERTY(double syncAcquisitionMinimum READ get_SyncAcquisitionMinimum WRITE set_SyncAcquisitionMinimum)
  QCEP_DOUBLE_PROPERTY(SyncAcquisitionMinimum)

  Q_PROPERTY(double syncAcquisitionMaximum READ get_SyncAcquisitionMaximum WRITE set_SyncAcquisitionMaximum)
  QCEP_DOUBLE_PROPERTY(SyncAcquisitionMaximum)

  Q_PROPERTY(double syncAcquisitionSymmetry READ get_SyncAcquisitionSymmetry WRITE set_SyncAcquisitionSymmetry)
  QCEP_DOUBLE_PROPERTY(SyncAcquisitionSymmetry)

  Q_PROPERTY(double syncAcquisitionPhaseShift READ get_SyncAcquisitionPhaseShift WRITE set_SyncAcquisitionPhaseShift)
  QCEP_DOUBLE_PROPERTY(SyncAcquisitionPhaseShift)

  Q_PROPERTY(double syncAcquisitionManualValue READ get_SyncAcquisitionManualValue WRITE set_SyncAcquisitionManualValue)
  QCEP_DOUBLE_PROPERTY(SyncAcquisitionManualValue)

signals:

public slots:
  void setManualOutput();
  void triggerOnce();

public:
  void prepareForAcquisition(QxrdAcquisitionParameterPack *parms);
  void prepareForDarkAcquisition(QxrdDarkAcquisitionParameterPack *parms);
  void finishedAcquisition();
  void acquiredFrameAvailable(int currentPhase);
  void setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin);
  QxrdNIDAQPluginInterfacePtr nidaqPlugin() const;

  QVector<double>  outputTimes();
  QVector<double>  outputVoltage();

private:
  mutable QMutex              m_Mutex;
  QxrdAcquisition            *m_Acquisition;
  QxrdAcquisitionParameterPack *m_AcquisitionParms;
  QxrdNIDAQPluginInterfacePtr m_NIDAQPlugin;
  int                         m_SyncMode;
  QVector<double>             m_OutputTimes;
  QVector<double>             m_OutputVoltage;
};

#endif // QXRDSYNCHRONIZEDACQUISITION_H
