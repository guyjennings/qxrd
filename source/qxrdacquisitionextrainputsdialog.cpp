#include "qxrdacquisitionextrainputsdialog.h"
#include "ui_qxrdacquisitionextrainputsdialog.h"
#include "qxrdacquisitionextrainputs.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

QxrdAcquisitionExtraInputsDialog::QxrdAcquisitionExtraInputsDialog(QWidget *parent, QxrdAcquisitionWPtr acq) :
  QDockWidget(parent),
  m_Acquisition(acq),
  m_AcquisitionExtraInputs()
{
  setupUi(this);

  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

    if (m_AcquisitionExtraInputs) {

      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

      if (xtra) {
        int nInputs = xtra->channels().count();

        m_ExtraInputsTable->setRowCount(nInputs);
        m_ExtraInputsTable->setColumnCount(7);

        for (int i=0; i<nInputs; i++) {
          setupUiChannel(i, xtra->channels().value(i));
        }
      }
    }
  }
}

void QxrdAcquisitionExtraInputsDialog::setupUiChannel(int i, QxrdAcquisitionExtraInputsChannelPtr ch)
{
  if (ch) {
    QCheckBox *cb = new QCheckBox();
    ch->prop_Enabled()->linkTo(cb);

    QLineEdit *le = new QLineEdit();
    ch->prop_Channel()->linkTo(le);

    QComboBox *md = new QComboBox();
    ch->prop_Mode()->linkTo(md);

    QDoubleSpinBox *min = new QDoubleSpinBox();
    ch->prop_Min()->linkTo(min);

    QDoubleSpinBox *max = new QDoubleSpinBox();
    ch->prop_Max()->linkTo(max);

    QDoubleSpinBox *stt = new QDoubleSpinBox();
    ch->prop_Start()->linkTo(stt);

    QDoubleSpinBox *end = new QDoubleSpinBox();
    ch->prop_Start()->linkTo(end);

    m_ExtraInputsTable->setCellWidget(i, 0, cb);
    m_ExtraInputsTable->setCellWidget(i, 1, le);
    m_ExtraInputsTable->setCellWidget(i, 2, md);
    m_ExtraInputsTable->setCellWidget(i, 3, min);
    m_ExtraInputsTable->setCellWidget(i, 4, max);
    m_ExtraInputsTable->setCellWidget(i, 5, stt);
    m_ExtraInputsTable->setCellWidget(i, 6, end);
  }
}

QxrdAcquisitionExtraInputsDialog::~QxrdAcquisitionExtraInputsDialog()
{
}
