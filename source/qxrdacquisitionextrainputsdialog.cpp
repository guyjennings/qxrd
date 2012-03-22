#include "qxrdacquisitionextrainputsdialog.h"
#include "ui_qxrdacquisitionextrainputsdialog.h"
#include "qxrdacquisitionextrainputs.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QMessageBox>

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

        connect(xtra.data(), SIGNAL(newDataAvailable()), this, SLOT(updateWaveforms()));
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

    QCheckBox *cb2 = new QCheckBox();
    ch->prop_Plotted()->linkTo(cb2);

    QLineEdit *le = new QLineEdit();
    ch->prop_ChannelName()->linkTo(le);

    QComboBox *md = new QComboBox();

    md->addItem("Sum");
    md->addItem("Avg");
    md->addItem("Max");
    md->addItem("Min");

    md->setItemData(0, "Value = Sum(wfm)", Qt::ToolTipRole);
    md->setItemData(1, "Value = Avg(wfm)", Qt::ToolTipRole);
    md->setItemData(2, "Value = Max(wfm)", Qt::ToolTipRole);
    md->setItemData(3, "Value = Min(wfm)", Qt::ToolTipRole);

    ch->prop_Mode()->linkTo(md);

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

    QLabel *phy = new QLabel();
    ch->prop_PhysicalChannel()->linkTo(phy);

    QLabel *val = new QLabel();
    ch->prop_Value()->linkTo(val);

    if (m_ChannelsInRows) {
      m_ExtraInputsTable->setCellWidget(i, 0, cb);
      m_ExtraInputsTable->setCellWidget(i, 1, cb2);
      m_ExtraInputsTable->setCellWidget(i, 2, le);
      m_ExtraInputsTable->setCellWidget(i, 3, md);
      m_ExtraInputsTable->setCellWidget(i, 4, wf);
      m_ExtraInputsTable->setCellWidget(i, 5, min);
      m_ExtraInputsTable->setCellWidget(i, 6, max);
      m_ExtraInputsTable->setCellWidget(i, 7, stt);
      m_ExtraInputsTable->setCellWidget(i, 8, end);
      m_ExtraInputsTable->setCellWidget(i, 9, phy);
      m_ExtraInputsTable->setCellWidget(i, 10, val);
    } else {
      m_ExtraInputsTable->setCellWidget(0, i, cb);
      m_ExtraInputsTable->setCellWidget(1, i, cb2);
      m_ExtraInputsTable->setCellWidget(2, i, le);
      m_ExtraInputsTable->setCellWidget(3, i, md);
      m_ExtraInputsTable->setCellWidget(4, i, wf);
      m_ExtraInputsTable->setCellWidget(5, i, min);
      m_ExtraInputsTable->setCellWidget(6, i, max);
      m_ExtraInputsTable->setCellWidget(7, i, stt);
      m_ExtraInputsTable->setCellWidget(8, i, end);
      m_ExtraInputsTable->setCellWidget(9, i, phy);
      m_ExtraInputsTable->setCellWidget(10, i, val);
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

        QStringList labels;

        labels << "Enabled" << "Plotted" << "Device Name" << "Mode" << "SaveWfm?"
               << "Min V" << "Max V"
               << "Start" << "End" << "Phys Chan" << "Value";

        QStringList chanLabels;

        for (int i=0; i<nInputs; i++) {
          chanLabels << tr("Chan %1").arg(i);
        }

        if (m_ChannelsInRows) {
          m_ExtraInputsTable->setRowCount(nInputs);
          m_ExtraInputsTable->setColumnCount(labels.count());
        } else {
          m_ExtraInputsTable->setRowCount(labels.count());
          m_ExtraInputsTable->setColumnCount(nInputs);
        }

        for (int i=0; i<nInputs; i++) {
          setupUiChannel(i, xtra->channels().value(i));
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
        int nch = xtra->channels().count();

        int n = (m_ChannelsInRows ?
                   m_ExtraInputsTable->currentRow() :
                   m_ExtraInputsTable->currentColumn());

        int ch = (n<0 ? nch-1 : n);

        QString titleString = tr("New Channel after Channel %1").arg(ch);
        QString questString = tr("Do you really want to create a new extra input channel after channel %1").arg(ch);

        int reply = QMessageBox::question(this, titleString, questString,
                                          QMessageBox::Ok | QMessageBox::Cancel,
                                          QMessageBox::Cancel);

        if (reply== QMessageBox::Ok) {
          if (ch < 0) {
            xtra->appendChannel();
          } else {
            xtra->appendChannel(ch);
          }

          updateUi();
        }
      }
    }
  }
}

void QxrdAcquisitionExtraInputsDialog::removeChannel()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

    if (m_AcquisitionExtraInputs) {

      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

      if (xtra) {
        int nch = xtra->channels().count();

        int n = (m_ChannelsInRows ?
                   m_ExtraInputsTable->currentRow() :
                   m_ExtraInputsTable->currentColumn());

        int ch = (n<0 ? nch-1 : n);

        QString titleString = tr("Delete Channel %1").arg(ch);
        QString questString = tr("Do you really want to delete extra input channel %1").arg(ch);

        int reply = QMessageBox::question(this, titleString, questString,
                                          QMessageBox::Ok | QMessageBox::Cancel,
                                          QMessageBox::Cancel);

        if (reply== QMessageBox::Ok) {
          if (n < 0) {
            xtra->removeChannel();
          } else {
            xtra->removeChannel(n);
          }

          updateUi();
        }
      }
    }
  }
}

void QxrdAcquisitionExtraInputsDialog::testReadout()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    acqp->printMessage(tr("Current column = %1").arg(m_ExtraInputsTable->currentColumn()));
    acqp->printMessage(tr("Current row = %1").arg(m_ExtraInputsTable->currentRow()));

    //  QList<QTableWidgetItem*> sel = m_ExtraInputsTable->selectedItems();

    //  printf("Selections\n");

    //  foreach(QTableWidgetItem *itm, sel) {
    //    if (itm) {
    //      printf("R: %d C: %d\n", itm->row(), itm->column());
    //    }
  }
}

void QxrdAcquisitionExtraInputsDialog::updateWaveforms()
{
  QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

  if (xtra) {
    int nchan = xtra->channels().count();

    m_AcquisitionWaveforms->setNChannels(nchan);

    for (int i=0; i<nchan; i++) {
      QxrdAcquisitionExtraInputsChannelPtr chanp(xtra->channel(i));

      if (chanp && chanp->get_Enabled() && chanp->get_Plotted()) {
        m_AcquisitionWaveforms->plotChannel(i,
                                            xtra->readXChannel(),
                                            xtra->readChannel(i));
      } else {
        m_AcquisitionWaveforms->plotChannel(i, QcepDoubleVector(), QcepDoubleVector());
      }
    }

    m_AcquisitionWaveforms->replot();
  }
}
