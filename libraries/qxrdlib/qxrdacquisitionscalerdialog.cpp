#include "qxrdacquisitionscalerdialog.h"
#include "ui_qxrdacquisitionscalerdialog.h"
#include "qxrdacqcommon.h"
#include "qxrdacquisitionscalermodel.h"

QxrdAcquisitionScalerDialog::QxrdAcquisitionScalerDialog(QxrdAcqCommonWPtr acq,
                                                         QWidget *parent) :
  QDockWidget(parent),
  m_Acquisition(acq),
  m_Model()
{
  QxrdAcqCommonPtr acquisition(m_Acquisition);

  if (acquisition) {
    m_Model = acquisition->acquisitionScalerModel();
  }

  setupUi(this);

  if (m_Model) {
    m_AcquisitionScalerView->setModel(m_Model.data());
    m_AcquisitionScalerView->horizontalHeader()->setStretchLastSection(true);

    m_AcquisitionScalerView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_AcquisitionScalerView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  }
}

QxrdAcquisitionScalerDialog::~QxrdAcquisitionScalerDialog()
{
}
