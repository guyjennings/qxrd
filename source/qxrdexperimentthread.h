#ifndef QXRDEXPERIMENTTHREAD_H
#define QXRDEXPERIMENTTHREAD_H

#include "qxrdthread.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdapplication-ptr.h"

class QxrdExperimentThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdExperimentThread(QString path, QxrdApplicationWPtr parent);
  ~QxrdExperimentThread();

  static QxrdExperimentThreadPtr newExperimentThread(QString path,
                                                     QxrdApplicationWPtr parent);

  void shutdown();

  QxrdExperimentPtr experiment() const;

protected:
  void run();

private:
  QxrdExperimentPtr   m_Experiment;
  QString             m_Path;
  QxrdApplicationWPtr m_Parent;
};

#endif // QXRDEXPERIMENTTHREAD_H
