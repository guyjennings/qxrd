#ifndef QXRDPERKINELMERDRIVER_H
#define QXRDPERKINELMERDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrdperkinelmersettings-ptr.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include "Acq.h"

class QxrdPerkinElmerDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdPerkinElmerDriver(QString name,
                        QxrdDetectorSettingsWPtr det,
                        QxrdExperimentWPtr expt,
                        QxrdAcqCommonWPtr acq);
  virtual ~QxrdPerkinElmerDriver();

public slots:
  bool startDetectorDriver();
  bool stopDetectorDriver();

  bool changeExposureTime(double exposure);
  bool beginAcquisition(double exposure);
  bool endAcquisition();
  void beginFrame();
  bool shutdownAcquisition();

  void onBinningModeChanged();
  void onCameraGainChanged();
  void onTimingSourceChanged();

private slots:
  void onEndFrame(int counter, unsigned int n1, unsigned int n2);

  void dumpHeaderInfo();

  QString acquisitionErrorString(int n);

public:
  void onEndFrameCallback();

private:
  void acquisitionError(const char *fn, int ln, int n);

  void startupAcquisition();
//  void setupCameraGainMenu(QComboBox *cb, int initialGain);
//  void setupCameraBinningModeMenu(QComboBox *cb, int initialBinning);
//  bool checkPluginAvailable();

  void acquisitionInitError(const char *fn, int ln, int n);
  void acquisitionNSensorsError(const char *fn, int ln, int n);

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
  int                    m_TimingSource;

  QAtomicInt             m_Counter;

  CHwHeaderInfo          m_HwHeaderInfo;
  CHwHeaderInfoEx        m_HwHeaderInfoEx;

private:
  QxrdPerkinElmerSettingsWPtr m_PerkinElmer;
};

#endif // QXRDPERKINELMERDRIVER_H
