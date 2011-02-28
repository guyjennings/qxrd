#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qcepmacros.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QAtomicInt>

#if QT_VERSION >= 0x040700
#include <QElapsedTimer>
#endif

#include "qxrdrasterdata.h"
#include "qxrdimagequeue.h"
#include "qxrdacquisitionoperations.h"

class QxrdAllocator;
class QxrdDataProcessor;
class QxrdSynchronizedAcquisition;
class QxrdNIDAQPluginInterface;
class QxrdAcquireDialog;
class QxrdWindow;

class QxrdAcquisition : public QxrdAcquisitionOperations
{
  Q_OBJECT;

public:
  QxrdAcquisition(QxrdDataProcessor *proc, QxrdAllocator *allocator);
  ~QxrdAcquisition();

public slots:
  virtual void initialize();

  void acquire();
  void acquireDark();
  void cancel();
  void cancelDark();
//  void trigger();
  void clearDropped();

  int acquisitionStatus(double time);

  virtual void onExposureTimeChanged(double newTime) = 0;
  virtual void onBinningModeChanged(int newMode) = 0;
  virtual void onCameraGainChanged(int newGain) = 0;
  void onBufferSizeChanged(int newMB);

signals:
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireStarted(int dark);
  void acquireComplete(int dark);

public:
  virtual void setupExposureMenu(QDoubleSpinBox *cb) = 0;
  virtual void setupCameraGainMenu(QComboBox *cb) = 0;
  virtual void setupCameraBinningModeMenu(QComboBox *cb) = 0;

  void indicateDroppedFrame();
  virtual QxrdAcquireDialog* controlPanel(QxrdWindow *win);

  QxrdSynchronizedAcquisition* synchronizedAcquisition() const;

  void setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin);
  QxrdNIDAQPluginInterface *nidaqPlugin() const;

protected:
  void allocateMemoryForAcquisition();
  void acquisition(int isDark);
  void copyParameters(int isDark);
  virtual void beginAcquisition();

  void acquisitionError(int n);
  void acquisitionError(int ln, int n);

  void acquireTiming();

  template <typename T>
  void accumulateAcquiredImage(QSharedPointer< QxrdImageData<T> > image, QxrdInt32ImageDataPtr accum, QxrdMaskDataPtr overflow);
  void processAcquiredImage(int fileIndex, int phase, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow);
  void getFileBaseAndName(int fileIndex, int phase, QString &fileBase, QString &fileName);

protected slots:
  virtual void haltAcquisition();
  void acquiredFrameAvailable(QxrdInt16ImageDataPtr image);

protected:
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

  QVector<QxrdInt32ImageDataPtr>  m_AcquiredInt32Data;
  QVector<QxrdMaskDataPtr>        m_OverflowMask;

  QAtomicInt             m_AcquireDark;

  QAtomicInt             m_NSkippedAtStart;
  QAtomicInt             m_NSkippedBetweenGroups;
  QAtomicInt             m_NPhasesPerSummation;
  QAtomicInt             m_NSummationsPerGroup;
  QAtomicInt             m_NGroupsPerSequence;

  QAtomicInt             m_FrameCounter;
  QAtomicInt             m_UpdateInterval;
  QAtomicInt             m_CurrentExposure;
  QAtomicInt             m_CurrentPhase;
  QAtomicInt             m_CurrentSummation;
  QAtomicInt             m_CurrentGroup;

  QAtomicInt             m_InitialFileIndex;

  QxrdAcquireDialog     *m_ControlPanel;

#if QT_VERSION >= 0x040700
  QElapsedTimer          m_ElapsedTimer;
#else
  QTime                  m_ElapsedTimer;
#endif
  QVector<int>           m_ElapsedHistogram;
  int                    m_ElapsedCounter;
};

#endif
