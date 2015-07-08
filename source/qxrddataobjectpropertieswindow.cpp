#include "qxrddataobjectpropertieswindow.h"
#include "qxrdexperiment.h"
#include "qcepdataobject.h"
#include "qcepdataobjectpropertiesmodel.h"

#include <QCloseEvent>
#include <QMessageBox>

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

  m_Model = QSharedPointer<QcepPropertiesModel>(
        new QcepDataObjectPropertiesModel(m_Object));

  m_TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_TableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  if (m_Model) {
    m_TableView->setModel(m_Model.data());
  }

  if (m_Object && m_Model) {
    connect(m_Object.data(), SIGNAL(dataObjectChanged()),
            m_Model.data(), SLOT(onDataObjectChanged()));
  }

  setAttribute(Qt::WA_DeleteOnClose, true);
}

void QxrdDataObjectPropertiesWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    event -> accept();
  } else {
    event -> ignore();
  }
}

bool QxrdDataObjectPropertiesWindow::wantToClose()
{
  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window %1 ?")
                               .arg(windowTitle()),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

