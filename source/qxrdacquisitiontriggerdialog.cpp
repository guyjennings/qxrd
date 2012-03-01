#include "qxrdacquisitiontriggerdialog.h"
#include "qxrdacquisition.h"
#include "ui_qxrdacquisitiontriggerdialog.h"
#include "qwt_plot_piecewise_curve.h"
#include "qxrdnidaqplugininterface.h"

QxrdAcquisitionTriggerDialog::QxrdAcquisitionTriggerDialog(QWidget *parent, QxrdAcquisitionWPtr acqw) :
  QDockWidget(parent),
  m_Acquisition(acqw),
  m_AcquisitionTrigger()
{
  setupUi(this);

  QxrdAcquisitionPtr acq(acqw);

  if (acq) {
    m_AcquisitionTrigger = acq->acquisitionTrigger();

    QxrdAcquisitionTriggerPtr trig(m_AcquisitionTrigger);
    QxrdNIDAQPluginInterfacePtr nidaq = acq->nidaqPlugin();

    m_ATrigMode     -> addItem("None");
    m_BTrigMode     -> addItem("None");

    if (nidaq && trig) {
      m_ATrigMode     -> addItem("Edge");
      m_ATrigMode     -> addItem("Level");

      m_BTrigMode     -> addItem("Edge");
      m_BTrigMode     -> addItem("Level");

      trig->prop_TriggerAMode()->linkTo(m_ATrigMode);
      trig->prop_TriggerBMode()->linkTo(m_BTrigMode);

      QStringList cards = nidaq -> deviceNames();

      for (int i=0; i<cards.count(); i++) {
        QString card = cards.value(i);
        QString type = nidaq->deviceType(card);
        int     sim  = nidaq->deviceIsSimulated(card);

        QString desc = card;

        desc += " : ";
        desc += type;

        if (sim) {
          desc += " [simulated]";
        }

        m_ATrigCard -> addItem(card);
        m_BTrigCard -> addItem(card);

        m_ATrigCard -> setItemData(i, desc, Qt::ToolTipRole);
        m_BTrigCard -> setItemData(i, desc, Qt::ToolTipRole);
      }

      trig->prop_TriggerACard()->linkTo(m_ATrigCard);
      trig->prop_TriggerBCard()->linkTo(m_BTrigCard);

      connect(trig->prop_TriggerACard(), SIGNAL(valueChanged(int,int)), this, SLOT(setATrigChannelNames()));
      connect(trig->prop_TriggerBCard(), SIGNAL(valueChanged(int,int)), this, SLOT(setBTrigChannelNames()));

      setATrigChannelNames();
      setBTrigChannelNames();

      trig->prop_TriggerAChannel()->linkTo(m_ATrigChan);
      trig->prop_TriggerBChannel()->linkTo(m_BTrigChan);

      m_ATrigSlope  -> addItem("Positive");
      m_ATrigSlope  -> addItem("Negative");

      m_BTrigSlope  -> addItem("Positive");
      m_BTrigSlope  -> addItem("Negative");

      trig->prop_TriggerASlope()->linkTo(m_ATrigSlope);
      trig->prop_TriggerBSlope()->linkTo(m_BTrigSlope);

      m_ATrigLevel  -> setMinimum(-10.0);
      m_ATrigLevel  -> setMaximum(10.0);
      m_ATrigLevel  -> setSingleStep(0.1);

      m_BTrigLevel  -> setMinimum(-10.0);
      m_BTrigLevel  -> setMaximum(10.0);
      m_BTrigLevel  -> setSingleStep(0.1);

      trig->prop_TriggerALevel()->linkTo(m_ATrigLevel);
      trig->prop_TriggerBLevel()->linkTo(m_BTrigLevel);

      m_ATrigHysteresis  -> setMinimum(0.0);
      m_ATrigHysteresis  -> setMaximum(10.0);
      m_ATrigHysteresis  -> setSingleStep(0.1);

      m_ATrigHysteresis  -> setMinimum(0.0);
      m_ATrigHysteresis  -> setMaximum(10.0);
      m_ATrigHysteresis  -> setSingleStep(0.1);

      trig->prop_TriggerAHysteresis()->linkTo(m_ATrigHysteresis);
      trig->prop_TriggerBHysteresis()->linkTo(m_BTrigHysteresis);

      trig->prop_TriggerAValue()->linkTo(m_ATrigValue);
      trig->prop_TriggerBValue()->linkTo(m_BTrigValue);

      trig->prop_TriggerAChannelName()->linkTo(m_ATrigChanName);
      trig->prop_TriggerBChannelName()->linkTo(m_BTrigChanName);

      connect(trig->prop_TriggerAChannel(), SIGNAL(valueChanged(int,int)),
              this, SLOT(triggerChanged()));
      connect(trig->prop_TriggerBChannel(), SIGNAL(valueChanged(int,int)),
              this, SLOT(triggerChanged()));
    } else {
      m_ATrigCard->setEnabled(false);
      m_BTrigCard->setEnabled(false);
      m_ATrigChan->setEnabled(false);
      m_BTrigChan->setEnabled(false);
      m_ATrigSlope->setEnabled(false);
      m_BTrigSlope->setEnabled(false);
      m_ATrigLevel->setEnabled(false);
      m_BTrigLevel->setEnabled(false);
      m_ATrigHysteresis->setEnabled(false);
      m_BTrigHysteresis->setEnabled(false);
      m_ATrigChanName->setEnabled(false);
      m_BTrigChanName->setEnabled(false);
    }
  }
}

QxrdAcquisitionTriggerDialog::~QxrdAcquisitionTriggerDialog()
{
}

void QxrdAcquisitionTriggerDialog::changeEvent(QEvent *e)
{
  QDockWidget::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdAcquisitionTriggerDialog::setATrigChannelNames()
{
  QxrdAcquisitionPtr acq(m_Acquisition);
  QxrdAcquisitionTriggerPtr trig(m_AcquisitionTrigger);

  if (acq && trig) {
    QxrdNIDAQPluginInterfacePtr nidaq(acq->nidaqPlugin());

    if (nidaq) {
      QStringList devices = nidaq->deviceNames();

      QString device = devices.value(trig->get_TriggerACard());

      QStringList channels = nidaq->deviceAIChannels(device);

      int nitems = m_ATrigChan->count();

      if (nitems > channels.count()) {
        for (int i=channels.count(); i<nitems; i++) {
          m_ATrigChan->removeItem(i);
        }
      } else if (nitems < channels.count()) {
        for (int i=nitems; i<channels.count(); i++) {
          m_ATrigChan->addItem(" ");
        }
      }

      for(int i=0; i<channels.count(); i++) {
        m_ATrigChan->setItemText(i, channels.value(i));
      }

      trig->set_TriggerAChannelName(m_ATrigChan->currentText());
    }
  }
}

void QxrdAcquisitionTriggerDialog::setBTrigChannelNames()
{
  QxrdAcquisitionPtr acq(m_Acquisition);
  QxrdAcquisitionTriggerPtr trig(m_AcquisitionTrigger);

  if (acq && trig) {
    QxrdNIDAQPluginInterfacePtr nidaq(acq->nidaqPlugin());

    if (nidaq) {
      QStringList devices = nidaq->deviceNames();

      QString device = devices.value(trig->get_TriggerBCard());

      QStringList channels = nidaq->deviceAIChannels(device);

      int nitems = m_BTrigChan->count();

      if (nitems > channels.count()) {
        for (int i=channels.count(); i<nitems; i++) {
          m_BTrigChan->removeItem(i);
        }
      } else if (nitems < channels.count()) {
        for (int i=nitems; i<channels.count(); i++) {
          m_BTrigChan->addItem(" ");
        }
      }

      for(int i=0; i<channels.count(); i++) {
        m_BTrigChan->setItemText(i, channels.value(i));
      }

      trig->set_TriggerBChannelName(m_BTrigChan->currentText());
    }
  }
}

void QxrdAcquisitionTriggerDialog::triggerChanged()
{
  QxrdAcquisitionPtr acq(m_Acquisition);
  QxrdAcquisitionTriggerPtr trig(m_AcquisitionTrigger);

  if (acq && trig) {
    trig -> set_TriggerAChannelName(m_ATrigChan->currentText());
    trig -> set_TriggerBChannelName(m_BTrigChan->currentText());
  }
}

