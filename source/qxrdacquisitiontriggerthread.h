//#ifndef QXRDACQUISITIONTRIGGERTHREAD_H
//#define QXRDACQUISITIONTRIGGERTHREAD_H

//#include "qxrdthread.h"
//#include "qxrdacquisitiontrigger-ptr.h"
//#include "qxrdsettingssaver-ptr.h"
//#include "qxrdexperiment-ptr.h"
//#include "qxrdacquisition-ptr.h"

//class QxrdAcquisitionTriggerThread : public QxrdThread
//{
//  Q_OBJECT

// public:
//  QxrdAcquisitionTriggerThread(QxrdSettingsSaverWPtr saver,
//                               QxrdExperimentWPtr    doc,
//                               QxrdAcquisitionWPtr   acq);

//  ~QxrdAcquisitionTriggerThread();

//  void shutdown();
//  void msleep(int msec);

//public:
//  QxrdAcquisitionTriggerPtr acquisitionTrigger() const;

//public:
//  void sleep(double time);

//protected:
//  void run();

//private:
//  int                       m_Debug;
//  QxrdSettingsSaverWPtr     m_Saver;
//  QxrdExperimentWPtr        m_Experiment;
//  QxrdAcquisitionWPtr       m_Acquisition;
//  QxrdAcquisitionTriggerPtr m_AcquisitionTrigger;
//};

//#endif
