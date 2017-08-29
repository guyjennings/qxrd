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
    m_AcquisitionScalerView->horizontalHeader()->setStretchLastSection(true);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    m_AcquisitionScalerView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_AcquisitionScalerView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    m_AcquisitionScalerView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    m_AcquisitionScalerView->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
  }
}

QxrdAcquisitionScalerDialog::~QxrdAcquisitionScalerDialog()
{
}
