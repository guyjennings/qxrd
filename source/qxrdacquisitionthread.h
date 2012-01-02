#ifndef QXRDACQUISITIONTHREAD_H
#define QXRDACQUISITIONTHREAD_H

#include "qxrdthread.h"
#include <QAtomicPointer>
#include <QStringList>
#include <QDateTime>
#include <QSettings>

class QxrdDataProcessor;
class QxrdAllocator;
class QxrdAcquisition;
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

//  void initialize();
  void shutdown();
  void msleep(int msec);

//  static QStringList detectorTypeNames();
//  static int detectorType();

public slots:
  void doAcquire();
  void cancel();

  void doAcquireDark();
  void cancelDark();

public:
  QxrdAcquisition *acquisition() const;

public:
  void sleep(double time);

protected:
  void run();

private:
  int                                 m_Debug;
  QxrdSettingsSaver                  *m_Saver;
  QxrdExperiment                     *m_Experiment;
  QxrdAllocator                      *m_Allocator;
  QAtomicPointer<QxrdAcquisition>     m_Acquisition;
  QxrdDataProcessor                  *m_Processor;
  int                                 m_DetectorType;
  QSettings                          *m_Settings;
  QString                             m_Section;
};

#endif
