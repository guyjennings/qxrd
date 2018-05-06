#include "qswexperimentthread.h"
#include "qswdebug.h"
#include "qswexperiment.h"

QswExperimentThread::QswExperimentThread(QString name)
  : inherited(name)
{
}

void QswExperimentThread::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QswExperimentThread::~QswExperimentThread()
{
  shutdown();
}

void QswExperimentThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printMessage("Experiment Thread Started");
  }

  m_Experiment =
      QswExperimentPtr(
        NEWPTR(QswExperiment("experiment")));

  m_Experiment -> initialize(sharedFromThis());

  int rc = exec();

  m_Experiment = QswExperimentPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printMessage(tr("Experiment Thread terminated with rc %1").arg(rc));
  }
}

QswExperimentWPtr QswExperimentThread::experiment()
{
  while (isRunning()) {
    if (m_Experiment) return m_Experiment;

    msleep(50);
  }

  return m_Experiment;
}
