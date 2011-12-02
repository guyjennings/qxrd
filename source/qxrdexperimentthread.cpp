#include "qxrdexperimentthread.h"
#include "qxrdapplication.h"
#include "qxrdexperimentperkinelmeracquisition.h"
#include "qxrdexperimentperkinelmeranalysis.h"
#include "qxrdexperimentpilatusacquisition.h"
#include "qxrdexperimentpilatusanalysis.h"
#include "qxrdexperimentsimulatedacquisition.h"
#include "qxrdexperimentgenericanalysis.h"
#include "qxrdsplashscreen.h"

QxrdExperimentThread::QxrdExperimentThread(QxrdExperimentKind kind, QString path, QxrdApplication *app) :
  QxrdThread(app),
  m_ExperimentKind(kind),
  m_ExperimentPath(path),
  m_Experiment(NULL),
  m_Application(app)
{
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPerkinElmerAcquisition(QString path, QxrdApplication *app)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PerkinElmerAcquisition, path, app));

  res->start();

  res->experiment()->init();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPilatusAcquisition(QString path, QxrdApplication *app)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PilatusAcquisition, path, app));

  res->start();

  res->experiment()->init();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentSimulatedAcquisition(QString path, QxrdApplication *app)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(SimulatedAcquisition, path, app));

  res->start();

  res->experiment()->init();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPerkinElmerAnalysis(QString path, QxrdApplication *app)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PerkinElmerAnalysis, path, app));

  res->start();

  res->experiment()->init();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPilatusAnalysis(QString path, QxrdApplication *app)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PilatusAnalysis, path, app));

  res->start();

  res->experiment()->init();

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentGenericAnalysis(QString path, QxrdApplication *app)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(GenericAnalysis, path, app));

  res->start();

  res->experiment()->init();

  return res;
}

QxrdExperimentPtr QxrdExperimentThread::experiment()
{
  while (m_Experiment == NULL) {
    msleep(100);
  }

  return m_Experiment;
}

void QxrdExperimentThread::run()
{
  QxrdSplashScreen splash;
  QxrdExperimentPtr doc;

  switch(m_ExperimentKind) {
  case PerkinElmerAcquisition:
    doc = QxrdExperimentPtr(new QxrdExperimentPerkinElmerAcquisition(m_ExperimentPath, m_Application));
    break;

  case PilatusAcquisition:
    doc = QxrdExperimentPtr(new QxrdExperimentPilatusAcquisition(m_ExperimentPath, m_Application));
    break;

  case SimulatedAcquisition:
    doc = QxrdExperimentPtr(new QxrdExperimentSimulatedAcquisition(m_ExperimentPath, m_Application));
    break;

  case PerkinElmerAnalysis:
    doc = QxrdExperimentPtr(new QxrdExperimentPerkinElmerAnalysis(m_ExperimentPath, m_Application));
    break;

  case PilatusAnalysis:
    doc = QxrdExperimentPtr(new QxrdExperimentPilatusAnalysis(m_ExperimentPath, m_Application));
    break;

  case GenericAnalysis:
    doc = QxrdExperimentPtr(new QxrdExperimentGenericAnalysis(m_ExperimentPath, m_Application));
    break;
  }

  m_Experiment = doc;
}

void QxrdExperimentThread::shutdown()
{
}
