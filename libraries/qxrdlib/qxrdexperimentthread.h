#ifndef QXRDEXPERIMENTTHREAD_H
#define QXRDEXPERIMENTTHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperimentsettings-ptr.h"

class QXRD_EXPORT QxrdExperimentThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdExperimentThread(QString path, QxrdApplicationWPtr parent, QxrdExperimentSettingsPtr set);
  ~QxrdExperimentThread();

  static QxrdExperimentThreadPtr newExperimentThread(QString path,
                                                     QxrdApplicationWPtr parent,
                                                     QxrdExperimentSettingsPtr set);

  void shutdown();

  QxrdExperimentPtr experiment() const;

protected:
  void run();

private:
  QxrdExperimentPtr         m_Experiment;
  QString                   m_Path;
  QxrdApplicationWPtr       m_Parent;
  QxrdExperimentSettingsPtr m_Settings;
};

#endif // QXRDEXPERIMENTTHREAD_H
