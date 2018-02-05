#include "qxrdacquisitionextrainputsdialog.h"
#include "ui_qxrdacquisitionextrainputsdialog.h"
#include "qxrdacquisitionextrainputs.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QMessageBox>
#include "qxrddebug.h"
#include "qxrdacquisitionextrainputsdialogsettings.h"
#include "qxrdacquisitionextrainputsplotsettings.h"

QxrdAcquisitionExtraInputsDialog::QxrdAcquisitionExtraInputsDialog(QxrdAcquisitionExtraInputsDialogSettingsWPtr set, QWidget *parent, QxrdAcquisitionWPtr acq) :
  QDockWidget(parent),
  m_Settings(set),
  m_Acquisition(acq),
  m_AcquisitionExtraInputs(),
  m_ChannelsInRows(0)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExtraInputsDialog::QxrdAcquisitionExtraInputsDialog(%p)\n", this);
  }

  setupUi(this);

  connect(m_AddChannel, &QAbstractButton::clicked, this, &QxrdAcquisitionExtraInputsDialog::addChannel);
  connect(m_RemoveChannel, &QAbstractButton::clicked, this, &QxrdAcquisitionExtraInputsDialog::removeChannel);
  connect(m_TestReadout, &QAbstractButton::clicked, this, &QxrdAcquisitionExtraInputsDialog::initiateReadout);

  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    m_AcquisitionExtraInputs = acqp->acquisitionExtraInputs();

    if (m_AcquisitionExtraInputs) {

      QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

      if (xtra) {
        QxrdNIDAQPluginInterfacePtr nidaq = xtra->nidaqPlugin();

        if (nidaq) {
          QStringList devices = nidaq->deviceNames();

          foreach(QString device, devices) {
            QString desc = nidaq->deviceType(device);
            int     isSim = nidaq->deviceIsSimulated(device);

            QString item = device+" : "+desc;

            if (isSim) {
              item += " [simulated]";
            }

            m_AcquisitionDevice->addItem(item, device);
          }
        }

        xtra->prop_SampleRate()->linkTo(m_SampleRate);
        xtra->prop_AcquireDelay()->linkTo(m_AcquisitionDelay);
        xtra->prop_DeviceName()->linkTo(m_AcquisitionDevice);

        connect(xtra->prop_DeviceName(), &QcepStringProperty::valueChanged,
                this, &QxrdAcquisitionExtraInputsDialog::updateUi);

        connect(xtra.data(), &QxrdAcquisitionExtraInputs::newDataAvailable,
                this, &QxrdAcquisitionExtraInputsDialog::updateWaveforms);

        connect(xtra.data(), &QxrdAcquisitionExtraInputs::channelCountChanged,
                this, &QxrdAcquisitionExtraInputsDialog::updateUi);
      }
    }
  }

  QxrdAcquisitionExtraInputsDialogSettingsPtr setg(m_Settings);

  if (setg) {
    m_AcquisitionWaveforms -> initialize(setg->plotSettings());
  }

  updateUi();
}

QxrdAcquisitionExtraInputsDialog::~QxrdAcquisitionExtraInputsDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExtraInputsDialog::~QxrdAcquisitionExtraInputsDialog(%p)\n", this);
  }
}

void QxrdAcquisitionExtraInputsDialog::setupUiChannel(int i, QxrdAcquisitionExtraInputsChannelPtr ch)
{
  if (ch) {
    QCheckBox *cb = new QCheckBox();
    ch->prop_Enabled()->linkTo(cb);

    QCheckBox *cb2 = new QCheckBox();
    ch->prop_Plotted()->linkTo(cb2);

    QComboBox *nm = new QComboBox();

    QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

    if (xtra) {
      QString deviceName = xtra->get_DeviceName();
      QxrdNIDAQPluginInterfacePtr nidaq = xtra->nidaqPlugin();

      if (nidaq) {
        QStringList aiChannels = nidaq->deviceAIChannels(deviceName);
        QStringList ctrChannels = nidaq->deviceCIChannels(deviceName);

        nm->addItem("Analog Inputs");
        foreach(QString chan, aiChannels) {
          nm->addItem(chan, chan);
        }

        nm->addItem("Counter Inputs");
        foreach(QString chan, ctrChannels) {
          nm->addItem(chan, chan);
        }
      }
    }

    ch->prop_ChannelName()->linkTo(nm);

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
    min->setSingleStep(0.1);
    ch->prop_Min()->linkTo(min);

    QDoubleSpinBox *max = new QDoubleSpinBox();
    max->setMinimum(-20.0);
    max->setMaximum(20.0);
    max->setSingleStep(0.1);
    ch->prop_Max()->linkTo(max);

    QDoubleSpinBox *stt = new QDoubleSpinBox();
    stt->setMinimum(-0.2);
    stt->setMaximum(10.0);
    stt->setSingleStep(0.1);
    ch->prop_Start()->linkTo(stt);

    QDoubleSpinBox *end = new QDoubleSpinBox();
    end->setMinimum(-0.2);
    end->setMaximum(10.0);
    end->setSingleStep(0.1);
    ch->prop_End()->linkTo(end);

    QComboBox  *tmode = new QComboBox();
    tmode -> addItem("No Trigger");
    tmode -> addItem("Positive Edge");
    tmode -> addItem("Negative Edge");
    tmode -> addItem("Positive Level");
    tmode -> addItem("Negative Level");
    ch->prop_TriggerMode()->linkTo(tmode);

    QDoubleSpinBox *tlevel = new QDoubleSpinBox();
    tlevel -> setMinimum(-20.0);
    tlevel -> setMaximum(+20.0);
    tlevel -> setSingleStep(0.1);
    ch->prop_TriggerLevel()->linkTo(tlevel);

    QDoubleSpinBox *thyst = new QDoubleSpinBox();
    thyst -> setMinimum(0.0);
    thyst -> setMaximum(+20.0);
    thyst -> setSingleStep(0.1);
    ch->prop_TriggerHysteresis()->linkTo(thyst);

    QLabel *phy = new QLabel();
    ch->prop_PhysicalChannel()->linkTo(phy);

    QLabel *val = new QLabel();
    ch->prop_Value()->linkTo(val);

    QCheckBox *trig = new QCheckBox();
    ch->prop_Triggered()->linkTo(trig);

    int c = 0;

    if (m_ChannelsInRows) {
      m_ExtraInputsTable->setCellWidget(i, c++, cb);
      m_ExtraInputsTable->setCellWidget(i, c++, cb2);
      m_ExtraInputsTable->setCellWidget(i, c++, nm);
      m_ExtraInputsTable->setCellWidget(i, c++, md);
      m_ExtraInputsTable->setCellWidget(i, c++, wf);
      m_ExtraInputsTable->setCellWidget(i, c++, min);
      m_ExtraInputsTable->setCellWidget(i, c++, max);
      m_ExtraInputsTable->setCellWidget(i, c++, stt);
      m_ExtraInputsTable->setCellWidget(i, c++, end);
      m_ExtraInputsTable->setCellWidget(i, c++, tmode);
      m_ExtraInputsTable->setCellWidget(i, c++, tlevel);
      m_ExtraInputsTable->setCellWidget(i, c++, thyst);
      m_ExtraInputsTable->setCellWidget(i, c++, phy);
      m_ExtraInputsTable->setCellWidget(i, c++, val);
      m_ExtraInputsTable->setCellWidget(i, c++, trig);
    } else {
      m_ExtraInputsTable->setCellWidget(c++, i, cb);
      m_ExtraInputsTable->setCellWidget(c++, i, cb2);
      m_ExtraInputsTable->setCellWidget(c++, i, nm);
      m_ExtraInputsTable->setCellWidget(c++, i, md);
      m_ExtraInputsTable->setCellWidget(c++, i, wf);
      m_ExtraInputsTable->setCellWidget(c++, i, min);
      m_ExtraInputsTable->setCellWidget(c++, i, max);
      m_ExtraInputsTable->setCellWidget(c++, i, stt);
      m_ExtraInputsTable->setCellWidget(c++, i, end);
      m_ExtraInputsTable->setCellWidget(c++, i, tmode);
      m_ExtraInputsTable->setCellWidget(c++, i, tlevel);
      m_ExtraInputsTable->setCellWidget(c++, i, thyst);
      m_ExtraInputsTable->setCellWidget(c++, i, phy);
      m_ExtraInputsTable->setCellWidget(c++, i, val);
      m_ExtraInputsTable->setCellWidget(c++, i, trig);
    }
  }
}

void QxrdAcquisitionExtraInputsDialog::updateUi()
{
  GUI_THREAD_CHECK;

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
               << "Start" << "End"
               << "Trigger Mode" << "Trigger Level" << "Trigger Hysteresis"
               << "Phys Chan" << "Value" << "Triggered?";

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

        if (m_ExtraInputsTable->columnCount() && m_ExtraInputsTable->rowCount()) {
          m_ExtraInputsTable->resizeColumnsToContents();
          m_ExtraInputsTable->resizeRowsToContents();
        }
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

void QxrdAcquisitionExtraInputsDialog::initiateReadout()
{
  QxrdAcquisitionExtraInputsPtr xtra(m_AcquisitionExtraInputs);

  if (xtra) {
    xtra->initiate();
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
                                            chanp->startIndex(),
                                            chanp->endIndex(),
                                            xtra->readXChannel(),
                                            xtra->readChannel(i));
      } else {
        m_AcquisitionWaveforms->plotChannel(i, 0, 0, QcepDoubleVector(), QcepDoubleVector());
      }
    }

    m_AcquisitionWaveforms->replot();
  }
}
