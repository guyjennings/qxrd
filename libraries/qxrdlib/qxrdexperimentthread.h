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

public:
  //TODO: pass parent pointer on initialization...
  QxrdExperimentThread(QString path,
                       QxrdAppCommonWPtr parent,
                       QxrdExperimentSettingsPtr set,
                       int mode);

  ~QxrdExperimentThread();

  static QxrdExperimentThreadWPtr findExperimentThread(QcepObjectWPtr parent);

  static QxrdExperimentThreadPtr newExperimentThread(QString path,
                                                     QxrdAppCommonWPtr parent,
                                                     QxrdExperimentSettingsPtr set,
                                                     int mode);

  void shutdown();

  QxrdExperimentPtr experiment() const;

protected:
  void run();

private:
  QxrdExperimentPtr         m_Experiment;
  QString                   m_Path;
  QxrdAppCommonWPtr         m_Parent;
  QxrdExperimentSettingsPtr m_Settings;
  int                       m_ExperimentMode;
};

#endif // QXRDEXPERIMENTTHREAD_H
