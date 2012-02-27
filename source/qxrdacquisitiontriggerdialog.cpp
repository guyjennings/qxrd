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

    if (nidaq && trig) {
      m_ATrigMode     -> addItem("None");
      m_ATrigMode     -> addItem("Edge");
      m_ATrigMode     -> addItem("Level");

      m_BTrigMode     -> addItem("None");
      m_BTrigMode     -> addItem("Edge");
      m_BTrigMode     -> addItem("Level");

      trig->prop_TriggerAMode()->linkTo(m_ATrigMode);
      trig->prop_TriggerBMode()->linkTo(m_BTrigMode);

      QStringList cards = nidaq -> deviceNames();

      foreach (QString card, cards) {
        m_ATrigCard -> addItem(card);
        m_BTrigCard -> addItem(card);
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
    }
  }
}
