#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qcepmacros.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QAtomicInt>
#include <QElapsedTimer>

#include "qxrdrasterdata.h"
#include "qxrdimagequeue.h"
#include "qxrdacquisitionoperations.h"

class QxrdSynchronizedAcquisition;
class QxrdNIDAQPluginInterface;

class QxrdAcquisition : public QxrdAcquisitionOperations
{
  Q_OBJECT;

public:
  QxrdAcquisition(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator);
  ~QxrdAcquisition();

public slots:
  virtual void initialize();

  void acquire();
  void acquireDark();
  void cancel();
  void cancelDark();
  void trigger();
  void clearDropped();

  void acquisitionReady();
  void acquisitionStarted();
  void acquisitionFinished();

  void darkAcquisitionStarted();

  int acquisitionStatus(double time);

  virtual void onExposureTimeChanged(double newTime) = 0;
  virtual void onBinningModeChanged(int newMode) = 0;
  virtual void onCameraGainChanged(int newGain) = 0;
  void onBufferSizeChanged(int newMB);

signals:
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);
  void acquireStarted(int dark);
  void acquireComplete(int dark);

public:
  virtual void setupExposureMenu(QDoubleSpinBox *cb) = 0;
  virtual void setupCameraGainMenu(QComboBox *cb) = 0;
  virtual void setupCameraBinningModeMenu(QComboBox *cb) = 0;

  void indicateDroppedFrame();
  virtual QxrdAcquireDialog* controlPanel(QxrdWindowPtr win);

  QxrdSynchronizedAcquisition* synchronizedAcquisition() const;

  void setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin);
  QxrdNIDAQPluginInterface *nidaqPlugin() const;

protected:
  void allocateMemoryForAcquisition();
  void acquisition(int isDark);
  void copyParameters(int isDark);
  virtual void beginAcquisition();
  void acquiredFrameAvailable();

  void acquisitionError(int n);
  void acquisitionError(int ln, int n);

  void acquireTiming();

protected slots:
  virtual void haltAcquisition();

protected:
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

//  QxrdInt16ImageQueue    m_FreeInt16Images;
//  QxrdInt32ImageQueue    m_FreeInt32Images;
  QxrdInt16ImageQueue    m_PreTriggerInt16Images;
  QxrdInt32ImageQueue    m_PreTriggerInt32Images;
  QVector<QxrdInt16ImageDataPtr>  m_AcquiredInt16Data;
  QVector<QxrdInt32ImageDataPtr>  m_AcquiredInt32Data;
  QxrdMaskDataPtr        m_OverflowMask;

  QAtomicInt             m_AcquireDark;
  QAtomicInt             m_NFramesStillToSkip;
  QAtomicInt             m_NFramesStillToSum;
  QAtomicInt             m_NPretriggerAcquired;
  QAtomicInt             m_NPostTriggerAcquired;
  QAtomicInt             m_CurrentExposure;
  QAtomicInt             m_CurrentFile;

  QxrdAcquireDialog     *m_ControlPanel;

  QxrdNIDAQPluginInterface    *m_NIDAQPlugin;
  QxrdSynchronizedAcquisition *m_SynchronizedAcquisition;

  QElapsedTimer          m_ElapsedTimer;
  QVector<int>           m_ElapsedHistogram;
  int                    m_ElapsedCounter;
};

#endif
