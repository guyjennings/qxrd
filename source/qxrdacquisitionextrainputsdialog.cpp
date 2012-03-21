#include "qxrdacquisitionextrainputsdialog.h"
#include "ui_qxrdacquisitionextrainputsdialog.h"
#include "qxrdacquisitionextrainputs.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

QxrdAcquisitionExtraInputsDialog::QxrdAcquisitionExtraInputsDialog(QWidget *parent, QxrdAcquisitionWPtr acq) :
  QDockWidget(parent),
  m_Acquisition(acq),
  m_AcquisitionExtraInputs(),
  m_ChannelsInRows(0)
{
  setupUi(this);

  connect(m_AddChannel, SIGNAL(clicked()), this, SLOT(addChannel()));
  connect(m_RemoveChannel, SIGNAL(clicked()), this, SLOT(removeChannel()));
  connect(m_TestReadout, SIGNAL(clicked()), this, SLOT(testReadout()));

  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

    if (m_AcquisitionExtraInputs) {

      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

      if (xtra) {
        xtra->prop_SampleRate()->linkTo(m_SampleRate);
        xtra->prop_AcquireDelay()->linkTo(m_AcquisitionDelay);
      }
    }
  }

  updateUi();
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

    md->addItem("Sum");
    md->addItem("Avg");
    md->addItem("Max");
    md->addItem("Min");

    md->setItemData(0, "Value = Sum(wfm)", Qt::ToolTipRole);
    md->setItemData(1, "Value = Avg(wfm)", Qt::ToolTipRole);
    md->setItemData(2, "Value = Max(wfm)", Qt::ToolTipRole);
    md->setItemData(3, "Value = Min(wfm)", Qt::ToolTipRole);

    QCheckBox *wf = new QCheckBox();
    ch->prop_SaveWave()->linkTo(wf);

    QDoubleSpinBox *min = new QDoubleSpinBox();
    min->setMinimum(-20.0);
    min->setMaximum(20.0);
    ch->prop_Min()->linkTo(min);

    QDoubleSpinBox *max = new QDoubleSpinBox();
    max->setMinimum(-20.0);
    max->setMaximum(20.0);
    ch->prop_Max()->linkTo(max);

    QDoubleSpinBox *stt = new QDoubleSpinBox();
    stt->setMinimum(-0.2);
    stt->setMaximum(10.0);
    ch->prop_Start()->linkTo(stt);

    QDoubleSpinBox *end = new QDoubleSpinBox();
    end->setMinimum(-0.2);
    end->setMaximum(10.0);
    ch->prop_Start()->linkTo(end);

    QLabel *val = new QLabel();
    ch->prop_Value()->linkTo(val);

    if (m_ChannelsInRows) {
      m_ExtraInputsTable->setCellWidget(i, 0, cb);
      m_ExtraInputsTable->setCellWidget(i, 1, le);
      m_ExtraInputsTable->setCellWidget(i, 2, md);
      m_ExtraInputsTable->setCellWidget(i, 3, wf);
      m_ExtraInputsTable->setCellWidget(i, 4, min);
      m_ExtraInputsTable->setCellWidget(i, 5, max);
      m_ExtraInputsTable->setCellWidget(i, 6, stt);
      m_ExtraInputsTable->setCellWidget(i, 7, end);
      m_ExtraInputsTable->setCellWidget(i, 8, val);
    } else {
      m_ExtraInputsTable->setCellWidget(0, i, cb);
      m_ExtraInputsTable->setCellWidget(1, i, le);
      m_ExtraInputsTable->setCellWidget(2, i, md);
      m_ExtraInputsTable->setCellWidget(3, i, wf);
      m_ExtraInputsTable->setCellWidget(4, i, min);
      m_ExtraInputsTable->setCellWidget(5, i, max);
      m_ExtraInputsTable->setCellWidget(6, i, stt);
      m_ExtraInputsTable->setCellWidget(7, i, end);
      m_ExtraInputsTable->setCellWidget(8, i, val);
    }
  }
}

QxrdAcquisitionExtraInputsDialog::~QxrdAcquisitionExtraInputsDialog()
{
}

void QxrdAcquisitionExtraInputsDialog::updateUi()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

    if (m_AcquisitionExtraInputs) {

      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

      if (xtra) {
        int nInputs = xtra->channels().count();

        m_ExtraInputsTable->clear();

        if (m_ChannelsInRows) {
          m_ExtraInputsTable->setRowCount(nInputs);
          m_ExtraInputsTable->setColumnCount(9);
        } else {
          m_ExtraInputsTable->setRowCount(9);
          m_ExtraInputsTable->setColumnCount(nInputs);
        }

        for (int i=0; i<nInputs; i++) {
          setupUiChannel(i, xtra->channels().value(i));
        }

        QStringList labels;

        labels << "Enabled" << "Device Name" << "Mode" << "SaveWfm?"
               << "Min V" << "Max V"
               << "Start" << "End" << "Value";

        QStringList chanLabels;

        for (int i=0; i<nInputs; i++) {
          chanLabels << tr("Chan %1").arg(i);
        }

        if (m_ChannelsInRows) {
          m_ExtraInputsTable->setHorizontalHeaderLabels(labels);
          m_ExtraInputsTable->setVerticalHeaderLabels(chanLabels);
        } else {
          m_ExtraInputsTable->setHorizontalHeaderLabels(chanLabels);
          m_ExtraInputsTable->setVerticalHeaderLabels(labels);
        }

        m_ExtraInputsTable->resizeColumnsToContents();
        m_ExtraInputsTable->resizeRowsToContents();
      }
    }
  }
}

void QxrdAcquisitionExtraInputsDialog::addChannel()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

    if (m_AcquisitionExtraInputs) {

      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

      if (xtra) {
        xtra->appendChannel();

        updateUi();
      }
    }
  }
//  QList<QTableWidgetItem*> sel = m_ExtraInputsTable->selectedItems();

//  printf("Selections\n");

//  foreach(QTableWidgetItem *itm, sel) {
//    if (itm) {
//      printf("R: %d C: %d\n", itm->row(), itm->column());
//    }
//  }
}

void QxrdAcquisitionExtraInputsDialog::removeChannel()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

    if (m_AcquisitionExtraInputs) {

      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

      if (xtra) {
        xtra->removeChannel();

        updateUi();
      }
    }
  }
}

void QxrdAcquisitionExtraInputsDialog::testReadout()
{
}
