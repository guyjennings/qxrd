#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include "qxrdthread.h"
#include "qxrdacquisition.h"
#include <QSettings>

class QxrdDataProcessor;
class QxrdAllocator;
class QxrdExperiment;
class QxrdSettingsSaver;

class QxrdAcquisitionThread : public QxrdThread
{
  Q_OBJECT

 public:
  QxrdAcquisitionThread(QxrdSettingsSaver *saver,
                        QxrdExperiment *doc,
                        QxrdDataProcessor *proc,
                        QxrdAllocator *allocator,
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
  int                                 m_Debug;
  QxrdSettingsSaver                  *m_Saver;
  QxrdExperiment                     *m_Experiment;
  QxrdAllocator                      *m_Allocator;
  QxrdAcquisitionPtr                  m_Acquisition;
  QxrdDataProcessor                  *m_Processor;
  int                                 m_DetectorType;
  QSettings                          *m_Settings;
  QString                             m_Section;
};

#endif
