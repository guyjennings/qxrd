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
#include "qxrdnidaqplugininterface-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrddetectorsettings-ptr.h"
//#include "qxrddetectorthread-ptr.h"
#include "qxrdprocessor-ptr.h"
//#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitionextrainputs-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdacquisitionscalermodel-ptr.h"
#include "qxrdacquisitionexecutionthread-ptr.h"
#include "qxrdacquisition-ptr.h"

class QXRD_EXPORT QxrdAcquisition : public QxrdAcqCommon
{
  Q_OBJECT

private:
  typedef QxrdAcqCommon inherited;

public:
  Q_INVOKABLE QxrdAcquisition(QString name);
  ~QxrdAcquisition();

  void initialize(QcepObjectWPtr parent);

  //TODO: change to experiment
//  void setWindow(QxrdWindowWPtr win);

  QxrdAcquisitionWPtr myself();

public slots:
  void acquire();
  void acquireOnce();
  void acquireDark();
  void cancel();
  void trigger();

  int acquisitionStatus(double time);

  void propertyList();
  void Message(QString cmd);

  void updateSaveTimes();

  void clearDropped();

  void shutdown();

  void doAcquire    ();
  void doAcquireOnce();
  void doAcquireDark();

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

  virtual void setupExposureMenu(QDoubleSpinBox *cb);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void indicateDroppedFrame(int n);
//  virtual QxrdAcquisitionDialogPtr controlPanel(QxrdWindowWPtr win);

  QxrdSynchronizedAcquisitionPtr synchronizedAcquisition() const;
  QxrdAcquisitionExtraInputsPtr acquisitionExtraInputs() const;

  void setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin);
  QxrdNIDAQPluginInterfaceWPtr nidaqPlugin() const;

protected:
//  void getFileBaseAndName(QString filePattern, QString extent, int detNum, int fileIndex, int phase, int nphases, QString &fileBase, QString &fileName);

  QxrdAppCommonWPtr application() const;
  QxrdExperimentWPtr experiment() const;
  QxrdProcessorWPtr processor() const;  //TODO: eliminate

protected slots:
  void onIdleTimeout();

private:
  void executeAcquisition(QxrdAcquisitionParameterPackPtr parmsp);
  virtual void stopIdling();
  virtual void startIdling();

//  template <typename TIMG, typename TACC>
//  void accumulateAcquiredImage(QSharedPointer< QcepImageData<TIMG> > image,
//                               QSharedPointer< QcepImageData<TACC> > accum,
//                               QcepMaskDataPtr overflow);

  void accumulateAcquiredImage(QcepUInt16ImageDataPtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepUInt32ImageDataPtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepUInt16ImageDataPtr image, QcepDoubleImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepUInt32ImageDataPtr image, QcepDoubleImageDataPtr accum, QcepMaskDataPtr overflow);

//  void processImage(const QxrdProcessArgs &args);
//  void processImage        (QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);
//  void processAcquiredImage(QString filePattern, QString extent, int fileIndex, int phase, int nPhases, bool trig, QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);
//  void processDarkImage    (QString filePattern, QString extent, int fileIndex,                                    QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);

  int cancelling();

protected:
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;

public:
private:
  QxrdAcquisitionExecutionThreadPtr m_ExecutionThread;

private:
  mutable QMutex                   m_Mutex;

  QxrdSynchronizedAcquisitionPtr   m_SynchronizedAcquisition;
  QxrdAcquisitionExtraInputsPtr    m_AcquisitionExtraInputs;

//  QxrdWindowWPtr                   m_Window;

//  QxrdAcquisitionDialogPtr         m_ControlPanel;

  QTimer                           m_IdleTimer;
  QAtomicInt                       m_Idling;

  QVector<QxrdDetectorSettingsPtr> m_Detectors;
};

Q_DECLARE_METATYPE(QxrdAcquisition*)

#endif
