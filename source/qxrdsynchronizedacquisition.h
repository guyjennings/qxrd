#ifndef QXRDSYNCHRONIZEDACQUISITION_H
#define QXRDSYNCHRONIZEDACQUISITION_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include "qcepproperty.h"
#include "qxrdsettings.h"

class QxrdAcquisition;
class QxrdNIDAQPluginInterface;

class QxrdSynchronizedAcquisition : public QObject
{
  Q_OBJECT
public:
  explicit QxrdSynchronizedAcquisition(QxrdAcquisition *acq);

public:
  Q_PROPERTY(int syncAcquisitionMode READ get_SyncAcquisitionMode WRITE set_SyncAcquisitionMode);
  QCEP_INTEGER_PROPERTY(SyncAcquisitionMode);

  enum {
    SyncAcquisitionModeNone,
    SyncAcquisitionModeStepped,
    SyncAcquisitionModeContinuous
  };

  Q_PROPERTY(int syncAcquisitionWaveform READ get_SyncAcquisitionWaveform WRITE set_SyncAcquisitionWaveform);
  QCEP_INTEGER_PROPERTY(SyncAcquisitionWaveform);

  enum {
    SyncAcquisitionWaveformSquare,
    SyncAcquisitionWaveformSine,
    SyncAcquisitionWaveformTriangle,
    SyncAcquisitionWaveformSawtooth
  };

  Q_PROPERTY(int syncAcquisitionOutputChannel READ get_SyncAcquisitionOutputChannel WRITE set_SyncAcquisitionOutputChannel);
  QCEP_INTEGER_PROPERTY(SyncAcquisitionOutputChannel);

  Q_PROPERTY(int syncAcquisitionFlagChannel READ get_SyncAcquisitionFlagChannel WRITE set_SyncAcquisitionFlagChannel);
  QCEP_INTEGER_PROPERTY(SyncAcquisitionFlagChannel);

  Q_PROPERTY(double syncAcquisitionMinimum READ get_SyncAcquisitionMinimum WRITE set_SyncAcquisitionMinimum);
  QCEP_DOUBLE_PROPERTY(SyncAcquisitionMinimum);

  Q_PROPERTY(double syncAcquisitionMaximum READ get_SyncAcquisitionMaximum WRITE set_SyncAcquisitionMaximum);
  QCEP_DOUBLE_PROPERTY(SyncAcquisitionMaximum);

  Q_PROPERTY(double syncAcquisitionSymmetry READ get_SyncAcquisitionSymmetry WRITE set_SyncAcquisitionSymmetry);
  QCEP_DOUBLE_PROPERTY(SyncAcquisitionSymmetry);

signals:

public slots:

public:
  void prepareForAcquisition();
  void acquiredFrameAvailable(int currentPhase);
  void setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin);
  QxrdNIDAQPluginInterface *nidaqPlugin() const;

  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

private:
  mutable QMutex             m_Mutex;
  QxrdAcquisition           *m_Acquisition;
  QxrdNIDAQPluginInterface  *m_NIDAQPlugin;
  int                        m_SyncMode;
  QVector<double>            m_OutputVoltage;
};

#endif // QXRDSYNCHRONIZEDACQUISITION_H
