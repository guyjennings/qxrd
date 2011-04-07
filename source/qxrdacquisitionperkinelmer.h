#ifndef QXRDACQUISITIONPERKINELMER_H
#define QXRDACQUISITIONPERKINELMER_H

//#ifdef HAVE_PERKIN_ELMER

#include "qxrdacquisition.h"
#include "qxrdperkinelmerplugininterface.h"

#include <QMutex>
#include <QVector>

class QxrdAcquisitionThread;

class QxrdAcquisitionPerkinElmer : public QxrdAcquisition
{
  Q_OBJECT;
public:
  QxrdAcquisitionPerkinElmer(QxrdDataProcessor *proc, QxrdAllocator *allocator);
  ~QxrdAcquisitionPerkinElmer();

private slots:
  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

public slots:
  void initialize();
  void onEndFrame(int counter, unsigned int n1, unsigned int n2);
  void onEndFrameCallback();

protected:
  void beginAcquisition();
  void endAcquisition();
  void shutdownAcquisition();

private:
  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);
  bool checkPluginAvailable();

  void acquisitionInitError(int n);
  void acquisitionNSensorsError(int n);

private:
  mutable QMutex         m_Mutex;
  int                    m_BufferSize;
  int                    m_BufferIndex;
  QVector<quint16>       m_Buffer;
  QVector<double>        m_ReadoutTimes;

  int                    m_PROMID;
  int                    m_HeaderID;
  int                    m_CameraType;
  QString                m_CameraModel;
  int                    m_CurrentGain;
  int                    m_SyncMode;

  QAtomicInt             m_Counter;

  QxrdPerkinElmerPluginInterface *m_PerkinElmer;
};

//#endif // HAVE_PERKIN_ELMER

#endif // QXRDACQUISITIONPERKINELMER_H
