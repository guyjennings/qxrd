#ifndef QXRDDETECTORDRIVERPERKINELMER_H
#define QXRDDETECTORDRIVERPERKINELMER_H

#include "qxrddetectordriver.h"
#include "qxrddetectorsettingsperkinelmer-ptr.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#ifdef HAVE_PERKIN_ELMER
#include "Acq.h"
#include "qxrdperkinelmerplugininterface-ptr.h"
#endif

class QxrdDetectorDriverPerkinElmer : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverPerkinElmer(QString name,
                                QxrdDetectorSettingsPerkinElmerPtr perkinElmer,
                                QxrdExperimentPtr expt,
                                QxrdAcquisitionPtr acq);
  virtual ~QxrdDetectorDriverPerkinElmer();

public slots:
  bool startDetectorDriver();
  bool stopDetectorDriver();

  bool changeExposureTime(double exposure);
  bool beginAcquisition(double exposure);
  bool endAcquisition();
  bool shutdownAcquisition();

  void onBinningModeChanged();
  void onCameraGainChanged();

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
  bool checkPluginAvailable();

  void acquisitionInitError(const char *fn, int ln, int n);
  void acquisitionNSensorsError(const char *fn, int ln, int n);

private:
#ifdef HAVE_PERKIN_ELMER
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

  QxrdPerkinElmerPluginInterfaceWPtr m_PerkinElmerPlugin;
#endif

private:
  QxrdDetectorSettingsPerkinElmerWPtr m_PerkinElmer;
};

#endif // QXRDDETECTORDRIVERPERKINELMER_H
