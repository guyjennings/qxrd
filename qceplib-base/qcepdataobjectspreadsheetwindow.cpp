#include "qcepdataobjectspreadsheetwindow.h"
#include "qcepexperiment.h"
#include "qcepdataobject.h"
#include <QCloseEvent>
#include <QMessageBox>

#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepimagedataspreadsheetmodel.h"

#include "qcepintegrateddata-ptr.h"
#include "qcepintegrateddata.h"
#include "qcepintegrateddataspreadsheetmodel.h"

#include "qcepdataarray.h"
#include "qcepdataarray-ptr.h"
#include "qcepdataarrayspreadsheetmodel.h"

#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepdatacolumnspreadsheetmodel.h"

#include "qcepdatacolumnscan.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepdatacolumnscanspreadsheetmodel.h"

#include "qcepdatagroup.h"
#include "qcepdatagroup-ptr.h"
#include "qcepdatagroupspreadsheetmodel.h"

QcepDataObjectSpreadsheetWindow::QcepDataObjectSpreadsheetWindow(QString name, QcepExperimentWPtr expt, QcepDataObjectWPtr obj) :
  inherited(name),
  m_Experiment(expt),
  m_Object(obj),
  m_Model()
{
  setupUi(this);

  QcepExperimentPtr e(m_Experiment);
  QcepDataObjectPtr objp(m_Object);

  if (objp && e) {
    setWindowTitle(tr("%1 Data from %2")
                   .arg(objp->pathName())
                   .arg(e->get_ExperimentName()));
  } else if (objp) {
    setWindowTitle(tr("%1 Data")
                   .arg(objp->pathName()));
  } else {
    setWindowTitle("Unknown Data");
  }

  if (objp) {
    QSharedPointer<QcepImageDataBase> data = qSharedPointerDynamicCast<QcepImageDataBase>(objp);

    if (data) {
      m_Model = QSharedPointer<QcepSpreadsheetModel>(
            new QcepImageDataSpreadsheetModel(data));
    } else {

      QcepIntegratedDataPtr integ = qSharedPointerDynamicCast<QcepIntegratedData>(m_Object);

      if (integ) {
        m_Model = QSharedPointer<QcepSpreadsheetModel>(
              new QcepIntegratedDataSpreadsheetModel(integ));
      } else {

        QcepDataArrayPtr array = qSharedPointerDynamicCast<QcepDataArray>(m_Object);

        if (array) {
          m_Model = QSharedPointer<QcepSpreadsheetModel>(
                new QcepDataArraySpreadsheetModel(array));
        } else {

          QcepDataColumnPtr col = qSharedPointerDynamicCast<QcepDataColumn>(m_Object);

          if (col) {
            m_Model = QSharedPointer<QcepSpreadsheetModel>(
                  new QcepDataColumnSpreadsheetModel(col));
          } else {
            QcepDataColumnScanPtr scan = qSharedPointerDynamicCast<QcepDataColumnScan>(m_Object);

            if (scan) {
              m_Model = QSharedPointer<QcepSpreadsheetModel>(
                    new QcepDataColumnScanSpreadsheetModel(scan));
            } else {
              QcepDataGroupPtr group = qSharedPointerDynamicCast<QcepDataGroup>(m_Object);

              if (group) {
                m_Model = QSharedPointer<QcepSpreadsheetModel>(
                      new QcepDataGroupSpreadsheetModel(group));
              }
            }
          }
        }
      }
    }
  }

//  m_TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  if (m_Model) {
    m_TableView->setModel(m_Model.data());
  }

  if (objp && m_Model) {
    connect(objp.data(), &QcepDataObject::dataObjectChanged, m_Model.data(), &QcepSpreadsheetModel::onDataObjectChanged);
  }

  setAttribute(Qt::WA_DeleteOnClose, true);
}

QcepDataObjectSpreadsheetWindow::~QcepDataObjectSpreadsheetWindow()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Spreadsheet Window\n");
#endif
}

void QcepDataObjectSpreadsheetWindow::closeEvent ( QCloseEvent * event )
{
  if (wantToClose()) {
    event -> accept();
  } else {
    event -> ignore();
  }
}

bool QcepDataObjectSpreadsheetWindow::wantToClose()
{
  return QMessageBox::question(this, tr("Really Close?"),
                               tr("Do you really want to close the window %1 ?")
                               .arg(windowTitle()),
                               QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
}

void QcepDataObjectSpreadsheetWindow::saveExperimentAs()
{
}

void QcepDataObjectSpreadsheetWindow::saveExperimentCopy()
{
}

void QcepDataObjectSpreadsheetWindow::doEditPreferences()
{
}
