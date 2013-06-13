#ifndef QXRDDETECTORPERKINELMER_H
#define QXRDDETECTORPERKINELMER_H

#include <QMutex>
#include <QDateTime>
#include "qcepmacros.h"
#include "qxrddetector.h"
#include "qxrdperkinelmerplugininterface-ptr.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include "Acq.h"

class QxrdDetectorPerkinElmer : public QxrdDetector
{
    Q_OBJECT
public:
  explicit QxrdDetectorPerkinElmer(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq);
  virtual ~QxrdDetectorPerkinElmer();
  int detectorType() const;

private slots:
  void onExposureTimeChanged();
  void onBinningModeChanged();
  void onCameraGainChanged();

public slots:
  void initialize();
  void onEndFrame(int counter, unsigned int n1, unsigned int n2);
  void onEndFrameCallback();

  void dumpHeaderInfo();

protected:
  void beginAcquisition();
  void endAcquisition();
  void shutdownAcquisition();

private:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void acquisitionError(const char *fn, int ln, int n);

  void startupAcquisition();
  void setupExposureMenu(QDoubleSpinBox *cb, double initialExposure);
  void setupCameraGainMenu(QComboBox *cb, int initialGain);
  void setupCameraBinningModeMenu(QComboBox *cb, int initialBinning);
  bool checkPluginAvailable();

  void acquisitionInitError(int n);
  void acquisitionNSensorsError(int n);

private:
  mutable QMutex         m_Mutex;
  int                    m_BufferSize;
  int                    m_BufferIndex;
  QVector<quint16>       m_Buffer;
  QVector<double>        m_ReadoutTimes;

  HACQDESC               m_AcqDesc;
  int                    m_StartupDelayed;

  int                    m_PROMID;
  int                    m_HeaderID;
  int                    m_CameraType;
  QString                m_CameraModel;
  int                    m_CurrentGain;
  int                    m_SyncMode;

  QAtomicInt             m_Counter;

  CHwHeaderInfo          m_HwHeaderInfo;
  CHwHeaderInfoEx        m_HwHeaderInfoEx;

  QxrdPerkinElmerPluginInterfaceWPtr m_PerkinElmer;
};

#endif // QXRDDETECTORPERKINELMER_H
