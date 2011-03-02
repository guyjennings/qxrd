#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qcepmacros.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QAtomicInt>
#include <QFutureWatcher>
#include <QTimer>

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

  class QxrdAcquisitionParameterPack
  {
  public:
    QxrdAcquisitionParameterPack(QString fileBase, double exposure, int nsummed, int nfiles, int nphases, int skipBefore, int skipBetween)
      : m_NFiles(nfiles), m_FileBase(fileBase), m_Exposure(exposure), m_NSummed(nsummed),
      m_NPhases(nphases), m_SkipBefore(skipBefore), m_SkipBetween(skipBetween) {}

    QString fileBase() { return m_FileBase; }
    double  exposure() { return m_Exposure; }
    int     nsummed()  { return m_NSummed; }
    int     nfiles()   { return m_NFiles; }
    int     nphases()  { return m_NPhases; }
    int     skipBefore() { return m_SkipBefore; }
    int     skipBetween() { return m_SkipBetween; }

  private:
    QString m_FileBase;
    double  m_Exposure;
    int     m_NSummed;
    int     m_NFiles;
    int     m_NPhases;
    int     m_SkipBefore;
    int     m_SkipBetween;
  };

  class QxrdDarkAcquisitionParameterPack
  {
  public:
    QxrdDarkAcquisitionParameterPack(QString fileBase, double exposure, int nsummed, int skipBefore)
      : m_FileBase(fileBase), m_Exposure(exposure), m_NSummed(nsummed), m_SkipBefore(skipBefore) {}

    QString fileBase() { return m_FileBase; }
    double  exposure() { return m_Exposure; }
    int     nsummed()  { return m_NSummed; }
    int     skipBefore() { return m_SkipBefore; }

  private:
    QString m_FileBase;
    double  m_Exposure;
    int     m_NSummed;
    int     m_SkipBefore;
  };

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

  void doAcquire    (QxrdAcquisitionParameterPack parms);
  void doAcquireDark(QxrdDarkAcquisitionParameterPack parms);

signals:
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireStarted(int dark = 0);
  void acquireComplete(int dark = 0);

public:
  int currentPhase(int frameNumber);

  virtual void setupExposureMenu(QDoubleSpinBox *cb) = 0;
  virtual void setupCameraGainMenu(QComboBox *cb) = 0;
  virtual void setupCameraBinningModeMenu(QComboBox *cb) = 0;

  void indicateDroppedFrame(int n);
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
  void getFileBaseAndName(QString filePattern, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName);

protected slots:
  virtual void haltAcquisition();
  void acquiredFrameAvailable(QxrdInt16ImageDataPtr image, int counter);
  void onAcquireComplete();
  void onIdleTimeout();

private:
  QxrdInt16ImageDataPtr acquireFrame(double exposure);
  virtual void stopIdling();
  virtual void startIdling();
  void processAcquiredImage(QString fileName, int fileIndex, int phase, QxrdInt32ImageDataPtr img, QxrdMaskDataPtr ovf);
  void processDarkImage(QString fileName, int fileIndex, QxrdInt32ImageDataPtr img, QxrdMaskDataPtr ovf);

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
//  QAtomicInt             m_CurrentExposure;
//  QAtomicInt             m_CurrentPhase;
//  QAtomicInt             m_CurrentSummation;
//  QAtomicInt             m_CurrentGroup;

  QAtomicInt             m_InitialFileIndex;

  QxrdAcquireDialog     *m_ControlPanel;

#if QT_VERSION >= 0x040700
  QElapsedTimer          m_ElapsedTimer;
#else
  QTime                  m_ElapsedTimer;
#endif
  QVector<int>           m_ElapsedHistogram;
  int                    m_ElapsedCounter;

  QFutureWatcher<void>   m_Watcher;

  QTimer                 m_IdleTimer;
  QAtomicInt             m_Idling;
};

#endif
