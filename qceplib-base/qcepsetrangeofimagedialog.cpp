#include "qcepsetrangeofimagedialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepsetrangeofimagecommand.h"

QcepSetRangeOfImageDialog::QcepSetRangeOfImageDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(NULL),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    m_Experiment = m_Model->experiment();

    if (m_Experiment) {
      m_Command = m_Experiment->setRangeOfImageCommand();
    }
  }

  if (m_Command) {
    m_ProgressIndicator->hide();

    m_RangeChoice->addItem("OUTSIDE: val<min || val>max");
    m_RangeChoice->addItem("INSIDE:  val>=min && val<=max");
    m_RangeChoice->addItem("ABOVE:   val>max");
    m_RangeChoice->addItem("BELOW:   val<min");

    m_RangeChoice->setCurrentIndex(m_Command->get_RangeChoice());

    m_Minimum->setRange(-qInf(), qInf());
    m_Minimum->setValue(m_Command->get_Minimum());

    m_Maximum->setRange(-qInf(), qInf());
    m_Maximum->setValue(m_Command->get_Maximum());

    m_OutputChoice->addItem("User defined value");
    m_OutputChoice->addItem("NaN");
    m_OutputChoice->addItem("INF");
    m_OutputChoice->addItem("-INF");

    m_OutputChoice->setCurrentIndex(m_Command->get_OutputChoice());

    connect(m_RangeChoice,  (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QcepSetRangeOfImageDialog::onRangeChoiceChanged);
    connect(m_OutputChoice, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QcepSetRangeOfImageDialog::onOutputChoiceChanged);

    onRangeChoiceChanged(m_Command->get_RangeChoice());
    onOutputChoiceChanged(m_Command->get_OutputChoice());
  }
}

QcepSetRangeOfImageDialog::~QcepSetRangeOfImageDialog()
{
}

void QcepSetRangeOfImageDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QcepSetRangeOfImageDialog::onRangeChoiceChanged(int newChoice)
{
  switch (newChoice) {
  case 0: // OUTSIDE
    m_Minimum->setEnabled(true);
    m_Maximum->setEnabled(true);
    break;

  case 1: // INSIDE
    m_Minimum->setEnabled(true);
    m_Maximum->setEnabled(true);
    break;

  case 2: // ABOVE
    m_Minimum->setEnabled(false);
    m_Maximum->setEnabled(true);
    break;

  case 3: // BELOW
    m_Minimum->setEnabled(true);
    m_Maximum->setEnabled(false);
    break;
  }
}

void QcepSetRangeOfImageDialog::onOutputChoiceChanged(int newChoice)
{
  m_NewValue->setEnabled(newChoice==0);
}

void QcepSetRangeOfImageDialog::accept()
{
  if (m_Command) {
    m_Command->set_RangeChoice(m_RangeChoice->currentIndex());
    m_Command->set_Minimum(m_Minimum->value());
    m_Command->set_Maximum(m_Maximum->value());
    m_Command->set_OutputChoice(m_OutputChoice->currentIndex());
    m_Command->set_NewValue(m_NewValue->value());

    QcepDoubleImageDataPtr input = m_Model->image(m_Index);

    if (input) {
      connect(m_Command.data(), &QcepSetRangeOfImageCommand::progressMade,
              m_ProgressIndicator, &QProgressBar::setValue);

      m_ProgressIndicator->show();

      QcepDoubleImageDataPtr mod = m_Command->exec(input);

      QModelIndex p = m_Model->parent(m_Index);

      m_Model->append(p, mod);
    }
  }

  QDialog::accept();
}
