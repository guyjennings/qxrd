#include "qxrdjsengine.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"

QxrdJSEngine::QxrdJSEngine(QxrdApplicationWPtr app, QxrdExperimentWPtr exp) :
  m_Application(app),
  m_Experiment(exp)
{
}

void QxrdJSEngine::initialize()
{
  globalObject().setProperty("global", globalObject());
  globalObject().setProperty("scripting", newQObject(this));

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    globalObject().setProperty("experiment", newQObject(expt.data()));

    QxrdAcquisitionPtr acq(expt->acquisition());

    if (acq) {
      globalObject().setProperty("acquisition", newQObject(acq.data()));
    }
  }
}

void QxrdJSEngine::setWindow(QxrdWindowWPtr win)
{
}
