#include "qxrddataobjectgraphwindow.h"
#include "qxrdexperiment.h"
#include "qcepdataobject.h"

QxrdDataObjectGraphWindow::QxrdDataObjectGraphWindow(
    QxrdExperimentWPtr expt, QcepDataObjectPtr obj, QWidget *parent) :
  QMainWindow(parent),
  m_Experiment(expt),
  m_Object(obj)
{
  setupUi(this);

  QxrdExperimentPtr e(m_Experiment);

  if (m_Object && expt) {
    setWindowTitle(tr("%1 Graph from %2")
                   .arg(m_Object->pathName())
                   .arg(e->get_ExperimentName()));
  } else if (m_Object) {
    setWindowTitle(tr("%1 Graph")
                   .arg(m_Object->pathName()));
  } else {
    setWindowTitle("Unknown Graph");
  }
}

