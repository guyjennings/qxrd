#include "qxrddataobjectspreadsheetwindow.h"
#include "qxrdexperiment.h"
#include "qcepdataobject.h"

#include "qxrdimagedata.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdimagedataspreadsheetmodel.h"

#include "qxrdintegrateddata-ptr.h"
#include "qxrdintegrateddata.h"
#include "qxrdintegrateddataspreadsheetmodel.h"

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

QxrdDataObjectSpreadsheetWindow::QxrdDataObjectSpreadsheetWindow(
    QxrdExperimentWPtr expt, QcepDataObjectPtr obj, QWidget *parent) :
  QMainWindow(parent),
  m_Experiment(expt),
  m_Object(obj),
  m_Model()
{
  setupUi(this);

  QxrdExperimentPtr e(m_Experiment);

  if (m_Object && e) {
    setWindowTitle(tr("%1 Data from %2")
                   .arg(m_Object->pathName())
                   .arg(e->get_ExperimentName()));
  } else if (m_Object) {
    setWindowTitle(tr("%1 Data")
                   .arg(m_Object->pathName()));
  } else {
    setWindowTitle("Unknown Data");
  }

  QSharedPointer<QcepImageDataBase> data = qSharedPointerDynamicCast<QcepImageDataBase>(m_Object);

  if (data) {
    m_Model = QSharedPointer<QcepSpreadsheetModel>(
          new QxrdImageDataSpreadsheetModel(data));
  } else {

    QxrdIntegratedDataPtr integ = qSharedPointerDynamicCast<QxrdIntegratedData>(m_Object);

    if (integ) {
      m_Model = QSharedPointer<QcepSpreadsheetModel>(
            new QxrdIntegratedDataSpreadsheetModel(integ));
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

  m_TableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  if (m_Model) {
    m_TableView->setModel(m_Model.data());
  }

  if (m_Object && m_Model) {
    connect(m_Object.data(), SIGNAL(dataObjectChanged()), m_Model.data(), SLOT(onDataObjectChanged()));
  }
}

