#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QAtomicInt>
#include <QTimer>
#include <QWaitCondition>
#include <QElapsedTimer>

#include "qxrdacqcommon.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdacquisitionscalermodel-ptr.h"
#include "qxrdacquisitionexecutionthread-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdacquisitioneventlog-ptr.h"
#include "qxrdsynchronizerthread-ptr.h"
#include "qxrdsynchronizer-ptr.h"

class QXRD_EXPORT QxrdAcquisition : public QxrdAcqCommon
{
  Q_OBJECT

private:
  typedef QxrdAcqCommon inherited;

public:
  Q_INVOKABLE QxrdAcquisition(QString name);
  ~QxrdAcquisition();

  void initialize(QcepObjectWPtr parent);
  void shutdown();

  static void registerMetaTypes();

public slots:
  void setupAcquisition();
  void acquire();
  void acquireOnce();
  void acquireDark();
  void cancel();
  void trigger();

  int acquisitionStatus(double time);

  void propertyList();

  void updateSaveTimes();

  void clearDropped();

//  void shutdown();

  void unlock();

  bool sanityCheckCommon();
  bool sanityCheckAcquire();
  bool sanityCheckAcquireDark();

  virtual void onExposureTimeChanged(double expos);

  QxrdDetectorSettingsPtr newDetector(int detType);
  void appendDetector(QxrdDetectorSettingsPtr det);
  void clearDetectors();

  void configureDetector(int i);
  void openDetectorControlWindow(int i);

  int                           detectorCount() const;
  QxrdDetectorSettingsPtr       detector(int n) const;
  QxrdDetectorSettings         *det(int n) const;

public:
  void openWindows();
  void closeWindows();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  QxrdSynchronizedAcquisitionPtr synchronizedAcquisition() const;

  QxrdSynchronizerWPtr synchronizer();

protected slots:
  void onIdleTimeout();

protected:
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

public:
private:
  QxrdAcquisitionExecutionThreadPtr m_ExecutionThread;

private:
  mutable QMutex                   m_Mutex;

  QxrdSynchronizedAcquisitionPtr   m_SynchronizedAcquisition;
  QxrdSynchronizerThreadPtr        m_SynchronizerThread;
  QxrdSynchronizerWPtr             m_Synchronizer;
  QTimer                           m_IdleTimer;
  QAtomicInt                       m_Idling;

  QVector<QxrdDetectorSettingsPtr> m_Detectors;
};

Q_DECLARE_METATYPE(QxrdAcquisition*)

#endif
