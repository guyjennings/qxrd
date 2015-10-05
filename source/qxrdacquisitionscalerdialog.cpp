#include "qxrdacquisitionscalerdialog.h"
#include "ui_qxrdacquisitionscalerdialog.h"
#include "qxrdacquisition.h"
#include "qxrdacquisitionscalermodel.h"

QxrdAcquisitionScalerDialog::QxrdAcquisitionScalerDialog(QxrdAcquisitionWPtr acq,
                                                         QWidget *parent) :
  QDockWidget(parent),
  m_Acquisition(acq),
  m_Model()
{
  QxrdAcquisitionPtr acquisition(m_Acquisition);

  if (acquisition) {
    m_Model = acquisition->acquisitionScalerModel();
  }

  setupUi(this);

  if (m_Model) {
    m_AcquisitionScalerView->setModel(m_Model.data());
  }
}

QxrdAcquisitionScalerDialog::~QxrdAcquisitionScalerDialog()
{
}
