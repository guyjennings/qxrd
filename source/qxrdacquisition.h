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
#include <QSemaphore>

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
    QxrdAcquisitionParameterPack(QString fileBase, double exposure, int nsummed, int preTrigger, int postTrigger, int nphases, int skipBefore, int skipBetween)
      : m_FileBase(fileBase), m_Exposure(exposure), m_NSummed(nsummed), m_PreTrigger(preTrigger), m_PostTrigger(postTrigger),
        m_NPhases(nphases), m_SkipBefore(skipBefore), m_SkipBetween(skipBetween) {}

    QString fileBase() { return m_FileBase; }
    double  exposure() { return m_Exposure; }
    int     nsummed()  { return m_NSummed; }
    int     preTrigger()   { return m_PreTrigger; }
    int     postTrigger()   { return m_PostTrigger; }
    int     nphases()  { return m_NPhases; }
    int     skipBefore() { return m_SkipBefore; }
    int     skipBetween() { return m_SkipBetween; }

  private:
    QString m_FileBase;
    double  m_Exposure;
    int     m_NSummed;
    int     m_PreTrigger;
    int     m_PostTrigger;
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
//  void cancelDark();
  void trigger();
  void clearDropped();

  void shutdown();


  int acquisitionStatus(double time);

  virtual void onExposureTimeChanged(double newTime) = 0;
  virtual void onBinningModeChanged(int newMode) = 0;
  virtual void onCameraGainChanged(int newGain) = 0;
  void onBufferSizeChanged(int newMB);

  void doAcquire    (QxrdAcquisitionParameterPack parms);
  void doAcquireDark(QxrdDarkAcquisitionParameterPack parms);

signals:
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireStarted();
  void acquireComplete();

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

  QxrdAcquisitionParameterPack acquisitionParameterPack();
  QxrdDarkAcquisitionParameterPack darkAcquisitionParameterPack();

protected:
  void allocateMemoryForAcquisition();
  void acquisition(int isDark);
  void copyParameters(int isDark);

  virtual void beginAcquisition() = 0;
  virtual void endAcquisition() = 0;
  virtual void shutdownAcquisition() = 0;

//  void acquisitionError(const char *fn, int n);
  void acquisitionError(const char *fn, int ln, int n);

  void getFileBaseAndName(QString filePattern, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName);

protected slots:
  void onAcquireComplete();
  void onIdleTimeout();

private:
  QxrdInt16ImageDataPtr acquireFrame(double exposure);
  QxrdInt16ImageDataPtr acquireFrameIfAvailable(double exposure);
  void flushImageQueue();

  virtual void stopIdling();
  virtual void startIdling();

  template <typename T>
  void accumulateAcquiredImage(QSharedPointer< QxrdImageData<T> > image, QxrdInt32ImageDataPtr accum, QxrdMaskDataPtr overflow);

  void processImage        (QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow);
  void processAcquiredImage(QString filePattern, int fileIndex, int phase, int nPhases, bool trig, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow);
  void processDarkImage    (QString filePattern, int fileIndex,                                    QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow);

  int cancelling();

protected:
  void enqueueAcquiredFrame(QxrdInt16ImageDataPtr img);

protected:
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

  QSemaphore             m_NAcquiredImages;
  QxrdInt16ImageQueue    m_AcquiredImages;

  QxrdAcquireDialog     *m_ControlPanel;

  QFutureWatcher<void>   m_Watcher;

  QTimer                 m_IdleTimer;
  QAtomicInt             m_Idling;
};

#endif
