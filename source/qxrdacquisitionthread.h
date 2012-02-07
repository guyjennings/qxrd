#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include "qxrdthread.h"
#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"
#include <QSettings>

class QxrdAllocator;
class QxrdExperiment;
class QxrdSettingsSaver;

class QxrdAcquisitionThread : public QxrdThread
{
  Q_OBJECT

 public:
  QxrdAcquisitionThread(QxrdSettingsSaverWPtr saver,
                        QxrdExperimentWPtr doc,
                        QxrdDataProcessorWPtr proc,
                        QxrdAllocatorWPtr allocator,
                        int detectorType,
                        QSettings *settings,
                        QString section);

  ~QxrdAcquisitionThread();

  void shutdown();
  void msleep(int msec);

public:
  QxrdAcquisitionPtr acquisition() const;

public:
  void sleep(double time);

protected:
  void run();

private:
  int                   m_Debug;
  QxrdSettingsSaverWPtr m_Saver;
  QxrdExperimentWPtr    m_Experiment;
  QxrdAllocatorWPtr     m_Allocator;
  QxrdAcquisitionWPtr   m_Acquisition;
  QxrdDataProcessorWPtr m_Processor;
  int                   m_DetectorType;
  QSettings            *m_Settings;
  QString               m_Section;
};

typedef QSharedPointer<QxrdAcquisitionThread> QxrdAcquisitionThreadPtr;

#endif
