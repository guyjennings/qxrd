#ifndef QXRDEXPERIMENTTHREAD_H
#define QXRDEXPERIMENTTHREAD_H

#include <QThread>
#include <QSharedPointer>
#include <QSettings>
#include "qxrdthread.h"

#include "qxrdexperiment-ptr.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperimentthread-ptr.h"

class QxrdExperimentThread : public QxrdThread, public QEnableSharedFromThis<QxrdExperimentThread>
{
  Q_OBJECT
public:
  QxrdExperimentThread(QString path, QxrdApplicationWPtr app, QSettings *settings);
  virtual ~QxrdExperimentThread();

  static QxrdExperimentThreadPtr newExperiment(QString path, QxrdApplicationWPtr app, QSettings *settings);

  QxrdExperimentPtr experiment();

  void shutdown();

signals:

public slots:

protected:
  void run();

private:
  QString             m_ExperimentPath;
  QxrdApplicationWPtr m_Application;
  QSettings          *m_Settings;
  QxrdExperimentPtr   m_Experiment;
};

typedef QSharedPointer<QxrdExperimentThread> QxrdExperimentThreadPtr;
typedef QWeakPointer<QxrdExperimentThread> QxrdExperimentThreadWPtr;

#endif // QXRDEXPERIMENTTHREAD_H
