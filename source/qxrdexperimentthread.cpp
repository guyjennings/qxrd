#include "qxrdexperimentthread.h"
#include "qxrdapplication.h"
#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentperkinelmeranalysis.h"
#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentpilatusanalysis.h"
#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentgenericanalysis.h"
#include "qxrdsplashscreen.h"

#include <stdio.h>

QxrdExperimentThread::QxrdExperimentThread(QxrdExperimentKind kind, QString path, QxrdApplication *app, QSettings *settings) :
  QxrdThread(),
  m_ExperimentKind(kind),
  m_ExperimentPath(path),
  m_Experiment(NULL),
  m_Application(app),
  m_Settings(settings)
{
}

QxrdExperimentThread::~QxrdExperimentThread()
{
  if (qcepDebug(DEBUG_APP)) {
    m_Application->printMessage("QxrdExperimentThread::~QxrdExperimentThread");
  }

  ::fflush(stdout);

  delete m_Experiment;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperiment(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(FromSettings, path, app, settings));

  res->start();

  res->experiment()->init(settings);

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPerkinElmerAcquisition(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PerkinElmerAcquisition, path, app, settings));

  res->start();

  res->experiment()->init(settings);

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPilatusAcquisition(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PilatusAcquisition, path, app, settings));

  res->start();

  res->experiment()->init(settings);

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentSimulatedAcquisition(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(SimulatedAcquisition, path, app, settings));

  res->start();

  res->experiment()->init(settings);

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPerkinElmerAnalysis(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PerkinElmerAnalysis, path, app, settings));

  res->start();

  res->experiment()->init(settings);

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPilatusAnalysis(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PilatusAnalysis, path, app, settings));

  res->start();

  res->experiment()->init(settings);

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentGenericAnalysis(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(GenericAnalysis, path, app, settings));

  res->start();

  res->experiment()->init(settings);

  return res;
}

QxrdExperiment *QxrdExperimentThread::experiment()
{
  while (m_Experiment == NULL) {
    msleep(100);
  }

  return m_Experiment;
}

void QxrdExperimentThread::run()
{
  QxrdExperiment *doc = NULL;

  if (m_ExperimentKind == FromSettings) {
    m_ExperimentKind = (QxrdExperimentKind) m_Settings->value("experiment/experimentKind",GenericAnalysis).toInt();
  }

  switch(m_ExperimentKind) {
  case PerkinElmerAcquisition:
    doc = new QxrdExperimentPerkinElmerAcquisition(m_ExperimentPath, m_Application, m_Settings);
    break;

  case PilatusAcquisition:
    doc = new QxrdExperimentPilatusAcquisition(m_ExperimentPath, m_Application, m_Settings);
    break;

  case SimulatedAcquisition:
    doc = new QxrdExperimentSimulatedAcquisition(m_ExperimentPath, m_Application, m_Settings);
    break;

  case PerkinElmerAnalysis:
    doc = new QxrdExperimentPerkinElmerAnalysis(m_ExperimentPath, m_Application, m_Settings);
    break;

  case PilatusAnalysis:
    doc = new QxrdExperimentPilatusAnalysis(m_ExperimentPath, m_Application, m_Settings);
    break;

  case GenericAnalysis:
    doc = new QxrdExperimentGenericAnalysis(m_ExperimentPath, m_Application, m_Settings);
    break;

  default:
    doc = new QxrdExperimentGenericAnalysis(m_ExperimentPath, m_Application, m_Settings);
    break;
  }

  if (doc) {
    m_Experiment = doc;

    int rc = exec();

    if (qcepDebug(DEBUG_THREADS)) {
      m_Application->printMessage(tr("Experiment Thread Terminated with rc %1").arg(rc));
    }
  }
}

void QxrdExperimentThread::shutdown()
{
//  INVOKE_CHECK(QMetaObject::invokeMethod(m_Experiment, "shutdown", Qt::QueuedConnection));
  exit();

  wait();
}
