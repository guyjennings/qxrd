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
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::QxrdExperimentThread(%p)\n", this);
  }
}

QxrdExperimentThread::~QxrdExperimentThread()
{
  if (qcepDebug(DEBUG_APP)) {
    m_Application->printMessage("QxrdExperimentThread::~QxrdExperimentThread");
  }

  ::fflush(stdout);

  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::~QxrdExperimentThread(%p)\n", this);
  }
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperiment(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(FromSettings, path, app, settings));

  res->start();

//  QxrdExperimentPtr expt(res->experiment());

//  if (expt) {
//    expt->init(res, res->experiment(), settings);
//  }

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPerkinElmerAcquisition(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PerkinElmerAcquisition, path, app, settings));

  res->start();

//  QxrdExperimentPtr expt(res->experiment());

//  if (expt) {
//    expt->init(res, res->experiment(), settings);
//  }

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPilatusAcquisition(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PilatusAcquisition, path, app, settings));

  res->start();

//  QxrdExperimentPtr expt(res->experiment());

//  if (expt) {
//    expt->init(res, res->experiment(), settings);
//  }

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentSimulatedAcquisition(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(SimulatedAcquisition, path, app, settings));

  res->start();

//  QxrdExperimentPtr expt(res->experiment());

//  if (expt) {
//    expt->init(res, res->experiment(), settings);
//  }

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPerkinElmerAnalysis(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PerkinElmerAnalysis, path, app, settings));

  res->start();

//  QxrdExperimentPtr expt(res->experiment());

//  if (expt) {
//    expt->init(res, res->experiment(), settings);
//  }

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentPilatusAnalysis(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(PilatusAnalysis, path, app, settings));

  res->start();

//  QxrdExperimentPtr expt(res->experiment());

//  if (expt) {
//    expt->init(res, res->experiment(), settings);
//  }

  return res;
}

QxrdExperimentThreadPtr QxrdExperimentThread::newExperimentGenericAnalysis(QString path, QxrdApplication *app, QSettings *settings)
{
  QxrdExperimentThreadPtr res = QxrdExperimentThreadPtr(new QxrdExperimentThread(GenericAnalysis, path, app, settings));

  res->start();

//  QxrdExperimentPtr expt(res->experiment());

//  if (expt) {
//    expt->init(res, res->experiment(), settings);
//  }

  return res;
}

QxrdExperimentWPtr QxrdExperimentThread::experiment()
{
  while (m_Experiment == NULL) {
    msleep(100);
  }

  return m_Experiment;
}

void QxrdExperimentThread::run()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdExperimentThread::run(%p)\n", this);
  }

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
          new QxrdExperimentPerkinElmerAcquisition(this, m_ExperimentPath, m_Application));
    break;

  case PilatusAcquisition:
    doc = QxrdExperimentPtr(
          new QxrdExperimentPilatusAcquisition(this, m_ExperimentPath, m_Application));
    break;

  case SimulatedAcquisition:
    doc = QxrdExperimentPtr(
          new QxrdExperimentSimulatedAcquisition(this, m_ExperimentPath, m_Application));
    break;

  case PerkinElmerAnalysis:
    doc = QxrdExperimentPtr(
          new QxrdExperimentPerkinElmerAnalysis(this, m_ExperimentPath, m_Application));
    break;

  case PilatusAnalysis:
    doc = QxrdExperimentPtr(
          new QxrdExperimentPilatusAnalysis(this, m_ExperimentPath, m_Application));
    break;

  case GenericAnalysis:
    doc = QxrdExperimentPtr(
          new QxrdExperimentGenericAnalysis(this, m_ExperimentPath, m_Application));
    break;

  default:
    doc = QxrdExperimentPtr(
          new QxrdExperimentGenericAnalysis(this, m_ExperimentPath, m_Application));
    break;
  }

  if (doc) {
    m_Experiment = doc;

    m_Experiment->readSettings(m_Settings);

    int rc = exec();

    if (qcepDebug(DEBUG_THREADS)) {
      m_Application->printMessage(tr("Experiment Thread Terminated with rc %1").arg(rc));
    }

    printf("Experiment thread terminated with rc %d\n", rc);
  }
}

void QxrdExperimentThread::shutdown()
{
//  INVOKE_CHECK(QMetaObject::invokeMethod(m_Experiment, "shutdown", Qt::QueuedConnection));
  exit();

  wait();
}
