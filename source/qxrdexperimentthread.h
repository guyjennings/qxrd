#ifndef QXRDEXPERIMENTTHREAD_H
#define QXRDEXPERIMENTTHREAD_H

#include <QThread>
#include <QSharedPointer>
#include <QSettings>
#include "qxrdthread.h"

#include "qxrdexperiment-ptr.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperimentthread-ptr.h"

class QxrdExperimentThread : public QxrdThread
{
  Q_OBJECT
public:
  typedef enum {
    FromSettings,
    PerkinElmerAcquisition,
    PilatusAcquisition,
    SimulatedAcquisition,
    PerkinElmerAnalysis,
    PilatusAnalysis,
    GenericAnalysis
  } QxrdExperimentKind;

  QxrdExperimentThread(QxrdExperimentKind kind, QString path, QxrdApplicationWPtr app, QSettings *settings);
  virtual ~QxrdExperimentThread();

  void init(QxrdExperimentThreadWPtr expThread);

  static QxrdExperimentThreadPtr newExperiment(QString path, QxrdApplicationWPtr app, QSettings *settings);
  static QxrdExperimentThreadPtr newExperimentPerkinElmerAcquisition(QString path, QxrdApplicationWPtr app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentPilatusAcquisition(QString path, QxrdApplicationWPtr app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentSimulatedAcquisition(QString path, QxrdApplicationWPtr app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentPerkinElmerAnalysis(QString path, QxrdApplicationWPtr app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentPilatusAnalysis(QString path, QxrdApplicationWPtr app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentGenericAnalysis(QString path, QxrdApplicationWPtr app, QSettings *settings=0);

  QxrdExperimentPtr experiment();

  void shutdown();

signals:

public slots:

protected:
  void run();

private:
  QxrdExperimentKind  m_ExperimentKind;
  QString             m_ExperimentPath;
  QxrdApplicationWPtr m_Application;
  QSettings          *m_Settings;
  QxrdExperimentThreadWPtr m_ExperimentThread;
  QxrdExperimentPtr   m_Experiment;
};

typedef QSharedPointer<QxrdExperimentThread> QxrdExperimentThreadPtr;
typedef QWeakPointer<QxrdExperimentThread> QxrdExperimentThreadWPtr;

#endif // QXRDEXPERIMENTTHREAD_H
