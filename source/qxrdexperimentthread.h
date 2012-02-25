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

  QxrdExperimentThread(QxrdExperimentKind kind, QString path, QxrdApplication *app, QSettings *settings);
  virtual ~QxrdExperimentThread();

  static QxrdExperimentThreadPtr newExperiment(QString path, QxrdApplication *app, QSettings *settings);
  static QxrdExperimentThreadPtr newExperimentPerkinElmerAcquisition(QString path, QxrdApplication *app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentPilatusAcquisition(QString path, QxrdApplication *app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentSimulatedAcquisition(QString path, QxrdApplication *app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentPerkinElmerAnalysis(QString path, QxrdApplication *app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentPilatusAnalysis(QString path, QxrdApplication *app, QSettings *settings=0);
  static QxrdExperimentThreadPtr newExperimentGenericAnalysis(QString path, QxrdApplication *app, QSettings *settings=0);

  QxrdExperimentWPtr experiment();

  void shutdown();

signals:

public slots:

protected:
  void run();

private:
  QxrdExperimentKind m_ExperimentKind;
  QString            m_ExperimentPath;
  QxrdExperimentPtr  m_Experiment;
  QxrdApplication   *m_Application;
  QSettings         *m_Settings;
};

typedef QSharedPointer<QxrdExperimentThread> QxrdExperimentThreadPtr;
typedef QWeakPointer<QxrdExperimentThread> QxrdExperimentThreadWPtr;

#endif // QXRDEXPERIMENTTHREAD_H
