#ifndef QXRDEXPERIMENTTHREAD_H
#define QXRDEXPERIMENTTHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdexperimentsettings-ptr.h"

class QXRD_EXPORT QxrdExperimentThread : public QxrdThread
{
  Q_OBJECT

private:
  typedef QxrdThread inherited;

public:
  //TODO: pass parent pointer on initialization...
  Q_INVOKABLE QxrdExperimentThread(QString name);

  ~QxrdExperimentThread();

  void initialize(QcepObjectWPtr            parent,
                  QString                   path,
                  QxrdExperimentSettingsPtr set,
                  int                       mode);

  static QxrdExperimentThreadWPtr findExperimentThread(QcepObjectWPtr parent);

//  static QxrdExperimentThreadPtr newExperimentThread(QString path,
//                                                     QcepObjectWPtr parent,
//                                                     QxrdExperimentSettingsPtr set,
//                                                     int mode);

  void shutdown();

  QxrdExperimentPtr experiment() const;

protected:
  void run();

private:
  QxrdExperimentPtr         m_Experiment;
  QString                   m_Path;
  QxrdExperimentSettingsPtr m_Settings;
  int                       m_ExperimentMode;
};

#endif // QXRDEXPERIMENTTHREAD_H
