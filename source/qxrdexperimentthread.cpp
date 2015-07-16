#include "qxrddebug.h"
#include "qxrdexperimentthread.h"
#include "qxrdapplication.h"
#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentperkinelmeranalysis.h"
#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentpilatusanalysis.h"
#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentgenericanalysis.h"
#include "qcepmutexlocker.h"

#include <stdio.h>

QxrdExperimentThread::QxrdExperimentThread(QxrdExperimentKind kind, QString path, QxrdApplicationWPtr app, QSettings *settings) :
  QxrdThread(),
  m_ExperimentKind(kind),
  m_ExperimentPath(path),
  m_Application(app),
  m_Settings(settings)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::QxrdExperimentThread(%p)\n", this);
  }
}

QxrdExperimentThread::~QxrdExperimentThread()
{
  if (qcepDebug(DEBUG_APP)) {
    QxrdApplicationPtr app(m_Application);

    if (app) {
      app->printMessage("QxrdExperimentThread::~QxrdExperimentThread");
    }
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::~QxrdExperimentThread(%p)\n", this);
  }

  ::fflush(stdout);

  shutdown();
}

void QxrdExperimentThread::init(QxrdExperimentThreadWPtr expThread)
{
  m_ExperimentThread = expThread;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperiment(QString path, QxrdApplicationWPtr app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(FromSettings, path, app, settings));

  res->start();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPerkinElmerAcquisition(QString path, QxrdApplicationWPtr app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PerkinElmerAcquisition, path, app, settings));

  res->start();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPilatusAcquisition(QString path, QxrdApplicationWPtr app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PilatusAcquisition, path, app, settings));

  res->start();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentSimulatedAcquisition(QString path, QxrdApplicationWPtr app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(SimulatedAcquisition, path, app, settings));

  res->start();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPerkinElmerAnalysis(QString path, QxrdApplicationWPtr app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PerkinElmerAnalysis, path, app, settings));

  res->start();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPilatusAnalysis(QString path, QxrdApplicationWPtr app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PilatusAnalysis, path, app, settings));

  res->start();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentGenericAnalysis(QString path, QxrdApplicationWPtr app, QSettings *settings)
{
  QxrdExperimentThreadPtr res =
      QxrdExperimentThreadPtr(new QxrdExperimentThread(GenericAnalysis, path, app, settings));

  res->start();

  return res;
}

QxrdExperimentPtr QxrdExperimentThread::experiment()
{
  while (isRunning()) {
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_Experiment) return m_Experiment;
    }

    msleep(100);
  }

  return QxrdExperimentPtr();
}

void QxrdExperimentThread::run()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::run(%p)\n", this);
  }

  {
    QxrdExperimentPtr doc;

    if (m_ExperimentKind == FromSettings) {
      if (m_Settings) {
        m_ExperimentKind = (QxrdExperimentKind) m_Settings->value("experiment/experimentKind",GenericAnalysis).toInt();
      } else {
        m_ExperimentKind = GenericAnalysis;
      }
    }

    switch(m_ExperimentKind) {
    case PerkinElmerAcquisition:
      doc = QxrdExperimentPtr(
            new QxrdExperimentPerkinElmerAcquisition(sharedFromThis(), m_ExperimentPath, m_Application));
      break;

    case PilatusAcquisition:
      doc = QxrdExperimentPtr(
            new QxrdExperimentPilatusAcquisition(sharedFromThis(), m_ExperimentPath, m_Application));
      break;

    case SimulatedAcquisition:
      doc = QxrdExperimentPtr(
            new QxrdExperimentSimulatedAcquisition(sharedFromThis(), m_ExperimentPath, m_Application));
      break;

    case PerkinElmerAnalysis:
      doc = QxrdExperimentPtr(
            new QxrdExperimentPerkinElmerAnalysis(sharedFromThis(), m_ExperimentPath, m_Application));
      break;

    case PilatusAnalysis:
      doc = QxrdExperimentPtr(
            new QxrdExperimentPilatusAnalysis(sharedFromThis(), m_ExperimentPath, m_Application));
      break;

    case GenericAnalysis:
      doc = QxrdExperimentPtr(
            new QxrdExperimentGenericAnalysis(sharedFromThis(), m_ExperimentPath, m_Application));
      break;

    default:
      doc = QxrdExperimentPtr(
            new QxrdExperimentGenericAnalysis(sharedFromThis(), m_ExperimentPath, m_Application));
      break;
    }

    if (doc) {
      doc -> initialize(m_Settings);

      m_Mutex.lock();
      m_Experiment = doc;
      m_Mutex.unlock();
    }
  }

  int rc = -1;

  rc = exec();

  {
    if (qcepDebug(DEBUG_THREADS)) {
      QxrdApplicationPtr app(m_Application);

      if (app) {
        app->printMessage(tr("Experiment Thread Terminated with rc %1").arg(rc));
      }
    }
  }
}

void QxrdExperimentThread::shutdown()
{
//  INVOKE_CHECK(QMetaObject::invokeMethod(m_Experiment, "shutdown", Qt::QueuedConnection));
  exit();

  wait();
}
