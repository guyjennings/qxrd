#include "qcepdataobjectpropertieswindow.h"
#include "qcepexperiment.h"
#include "qcepdataobject.h"
#include "qcepdataobjectpropertiesmodel.h"

#include <QCloseEvent>
#include <QMessageBox>

QcepDataObjectPropertiesWindow::QcepDataObjectPropertiesWindow
  (QcepExperimentWPtr expt, QcepDataObjectWPtr obj, QWidget *parent) :
  QcepDataObjectWindow(parent),
  m_Experiment(expt),
  m_Object(obj)
{
  setupUi(this);

  QcepExperimentPtr e(m_Experiment);
  QcepDataObjectPtr objp(m_Object);

  if (objp && e) {
    setWindowTitle(tr("%1 Properties from %2")
                   .arg(objp->pathName())
                   .arg(e->get_ExperimentName()));
  } else if (objp) {
    setWindowTitle(tr("%1 Properties")
                   .arg(objp->pathName()));
  } else {
    setWindowTitle("Unknown Properties");
  }

  if (objp) {
    m_Model = QSharedPointer<QcepPropertiesModel>(
          new QcepDataObjectPropertiesModel(m_Object));
  }

  m_TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_TableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  if (m_Model) {
    m_TableView->setModel(m_Model.data());
  }

  if (objp && m_Model) {
    connect(objp.data(), SIGNAL(dataObjectChanged()),
            m_Model.data(), SLOT(onDataObjectChanged()));
  }

  setAttribute(Qt::WA_DeleteOnClose, true);
}

QcepDataObjectPropertiesWindow::~QcepDataObjectPropertiesWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Properties Window\n");
#endif
}

void QcepDataObjectPropertiesWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    event -> accept();
  } else {
    event -> ignore();
  }
}

bool QcepDataObjectPropertiesWindow::wantToClose()
{
  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window %1 ?")
                               .arg(windowTitle()),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

