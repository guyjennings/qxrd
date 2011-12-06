#ifndef QXRDEXPERIMENTTHREAD_H
#define QXRDEXPERIMENTTHREAD_H

#include <QThread>
#include <QSharedPointer>
#include "qxrdthread.h"
#include "qxrdexperiment.h"

class QxrdExperimentThread;
//typedef QSharedPointer<QxrdExperimentThread> QxrdExperimentThreadPtr;
typedef QxrdExperimentThread *QxrdExperimentThreadPtr;

class QxrdExperimentThread : public QxrdThread
{
  Q_OBJECT
public:
  typedef enum {
    PerkinElmerAcquisition,
    PilatusAcquisition,
    SimulatedAcquisition,
    PerkinElmerAnalysis,
    PilatusAnalysis,
    GenericAnalysis
  } QxrdExperimentKind;

  QxrdExperimentThread(QxrdExperimentKind kind, QString path, QxrdApplication *app);
  virtual ~QxrdExperimentThread();

  static QxrdExperimentThreadPtr newExperimentPerkinElmerAcquisition(QString path, QxrdApplication *app);
  static QxrdExperimentThreadPtr newExperimentPilatusAcquisition(QString path, QxrdApplication *app);
  static QxrdExperimentThreadPtr newExperimentSimulatedAcquisition(QString path, QxrdApplication *app);
  static QxrdExperimentThreadPtr newExperimentPerkinElmerAnalysis(QString path, QxrdApplication *app);
  static QxrdExperimentThreadPtr newExperimentPilatusAnalysis(QString path, QxrdApplication *app);
  static QxrdExperimentThreadPtr newExperimentGenericAnalysis(QString path, QxrdApplication *app);

  QxrdExperiment *experiment();

  void shutdown();

signals:

public slots:

protected:
  void run();

private:
  QxrdExperimentKind m_ExperimentKind;
  QString            m_ExperimentPath;
  QxrdExperiment    *m_Experiment;
  QxrdApplication   *m_Application;
};

#endif // QXRDEXPERIMENTTHREAD_H
