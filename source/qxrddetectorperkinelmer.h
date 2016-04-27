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
  explicit QxrdDetectorPerkinElmer(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum, QcepObjectWPtr parent);
  virtual ~QxrdDetectorPerkinElmer();

  int detectorNumber() const;
  int detectorSubType() const;
  QString detectorAddress() const;

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

public slots:
  void onExposureTimeChanged();
  void onBinningModeChanged();
  void onCameraGainChanged();

public slots:
  void startDetector();
  void stopDetector();
  void onEndFrame(int counter, unsigned int n1, unsigned int n2);
  void onEndFrameCallback();

  void dumpHeaderInfo();

  QString acquisitionErrorString(int n);

protected:
  void beginAcquisition(double exposure);
  void endAcquisition();
  void shutdownAcquisition();

private:
  void acquisitionError(const char *fn, int ln, int n);

  void startupAcquisition();
//  void setupCameraGainMenu(QComboBox *cb, int initialGain);
//  void setupCameraBinningModeMenu(QComboBox *cb, int initialBinning);
  bool checkPluginAvailable();

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

  QAtomicInt             m_Counter;

  CHwHeaderInfo          m_HwHeaderInfo;
  CHwHeaderInfoEx        m_HwHeaderInfoEx;

  QxrdPerkinElmerPluginInterfaceWPtr m_PerkinElmer;

public:
  Q_PROPERTY(int detectorNumber READ get_DetectorNumber WRITE set_DetectorNumber)
  QCEP_INTEGER_PROPERTY(DetectorNumber)

  Q_PROPERTY(int detectorSubType READ get_DetectorSubType WRITE set_DetectorSubType)
  QCEP_INTEGER_PROPERTY(DetectorSubType)

  Q_PROPERTY(QString detectorAddress READ get_DetectorAddress WRITE set_DetectorAddress)
  QCEP_STRING_PROPERTY(DetectorAddress)

  Q_PROPERTY(int detectorGain READ get_DetectorGain WRITE set_DetectorGain)
  QCEP_INTEGER_PROPERTY(DetectorGain)

  Q_PROPERTY(int detectorBinning READ get_DetectorBinning WRITE set_DetectorBinning)
  QCEP_INTEGER_PROPERTY(DetectorBinning)
};

#endif // QXRDDETECTORPERKINELMER_H
