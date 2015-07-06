#include "qxrddataobjectpropertieswindow.h"
#include "qxrdexperiment.h"
#include "qcepdataobject.h"

QxrdDataObjectPropertiesWindow::QxrdDataObjectPropertiesWindow
  (QxrdExperimentWPtr expt, QcepDataObjectPtr obj, QWidget *parent) :
  QMainWindow(parent),
  m_Experiment(expt),
  m_Object(obj)
{
  setupUi(this);

  QxrdExperimentPtr e(m_Experiment);

  if (m_Object && e) {
    setWindowTitle(tr("%1 Properties from %2")
                   .arg(m_Object->pathName())
                   .arg(e->get_ExperimentName()));
  } else if (m_Object) {
    setWindowTitle(tr("%1 Properties")
                   .arg(m_Object->pathName()));
  } else {
    setWindowTitle("Unknown Properties");
  }
}
