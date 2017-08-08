#include "qcepsetdatavaluerangedialog.h"
#include "qcepdatasetmodel.h"
#include "qcepexperiment.h"
#include "qcepsetdatavaluerangecommand.h"

QcepSetDataValueRangeDialog::QcepSetDataValueRangeDialog(QcepDatasetModelPtr model, const QModelIndex &idx) :
  QDialog(NULL),
  m_Model(model),
  m_Index(idx)
{
  setupUi(this);

  if (m_Model) {
    m_Experiment = m_Model->experiment();

    if (m_Experiment) {
      m_Command = m_Experiment->setDataValueRangeCommand();
    }
  }

  if (m_Command) {
    m_ProgressIndicator -> hide();

    m_RegionType->addItem("Points inside rectangle");
    m_RegionType->addItem("Points outside rectangle");
    m_RegionType->addItem("Points inside ellipse");
    m_RegionType->addItem("Points outside ellipse");
    m_RegionType->setCurrentIndex(m_Command->get_RegionType());

    m_RegionCoordinateStyle->addItem("Left,Top,Right,Bottom");
    m_RegionCoordinateStyle->addItem("CentreX,CentreY,Width,Height");
    m_RegionCoordinateStyle->addItem("BorderLeft,BorderTop,BorderRight,BorderBottom");
    m_RegionCoordinateStyle->setCurrentIndex(m_Command->get_RegionCoordinateStyle());

    m_RegionX1->setRange(-qInf(), qInf());
    m_RegionX1->setValue(m_Command->get_RegionX1());

    m_RegionY1->setRange(-qInf(), qInf());
    m_RegionY1->setValue(m_Command->get_RegionY1());

    m_RegionX2->setRange(-qInf(), qInf());
    m_RegionX2->setValue(m_Command->get_RegionX2());

    m_RegionY2->setRange(-qInf(), qInf());
    m_RegionY2->setValue(m_Command->get_RegionY2());

    m_OutputChoice->addItem("User defined value");
    m_OutputChoice->addItem("NaN");
    m_OutputChoice->addItem("INF");
    m_OutputChoice->addItem("-INF");
    m_OutputChoice->setCurrentIndex(m_Command->get_OutputChoice());

    m_NewValue->setRange(-qInf(), qInf());
    m_NewValue->setValue(m_Command->get_NewValue());

    connect(m_RegionType,             (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QcepSetDataValueRangeDialog::onRegionTypeChanged);
    connect(m_RegionCoordinateStyle,  (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QcepSetDataValueRangeDialog::onRegionCoordinateStyleChanged);
    connect(m_OutputChoice,           (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged, this, &QcepSetDataValueRangeDialog::onOutputChoiceChanged);

    onRegionTypeChanged(m_Command->get_RegionType());
    onRegionCoordinateStyleChanged(m_Command->get_RegionCoordinateStyle());
    onOutputChoiceChanged(m_Command->get_OutputChoice());
  }
}

QcepSetDataValueRangeDialog::~QcepSetDataValueRangeDialog()
{
}

void QcepSetDataValueRangeDialog::changeEvent(QEvent *e)
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

void QcepSetDataValueRangeDialog::onRegionTypeChanged(int newChoice)
{
}

void QcepSetDataValueRangeDialog::onRegionCoordinateStyleChanged(int newChoice)
{
  switch (newChoice) {
  case 0:
    m_RegionLabelX1->setText("Left");
    m_RegionLabelY1->setText("Top");
    m_RegionLabelX2->setText("Right");
    m_RegionLabelY2->setText("Bottom");
    break;

  case 1:
    m_RegionLabelX1->setText("Center X");
    m_RegionLabelY1->setText("Center Y");
    m_RegionLabelX2->setText("Width");
    m_RegionLabelY2->setText("Height");
    break;

  case 2:
    m_RegionLabelX1->setText("Left Border");
    m_RegionLabelY1->setText("Top Border");
    m_RegionLabelX2->setText("Right Border");
    m_RegionLabelY2->setText("Botton Border");
    break;
  }
}

void QcepSetDataValueRangeDialog::onOutputChoiceChanged(int newChoice)
{
  m_NewValue->setEnabled(newChoice==0);
}

void QcepSetDataValueRangeDialog::accept()
{
  if (m_Command) {
    m_Command->set_RegionType(m_RegionType->currentIndex());
    m_Command->set_RegionCoordinateStyle(m_RegionCoordinateStyle->currentIndex());
    m_Command->set_RegionX1(m_RegionX1->value());
    m_Command->set_RegionY1(m_RegionY1->value());
    m_Command->set_RegionX2(m_RegionX2->value());
    m_Command->set_RegionY2(m_RegionY2->value());
    m_Command->set_OutputChoice(m_OutputChoice->currentIndex());
    m_Command->set_NewValue(m_NewValue->value());

    QcepDoubleImageDataPtr input = m_Model->image(m_Index);

    if (input) {
      connect(m_Command.data(), &QcepSetDataValueRangeCommand::progressMade,
              m_ProgressIndicator, &QProgressBar::setValue);

      m_ProgressIndicator->show();

      QcepDoubleImageDataPtr mod = m_Command->exec(input);

      QModelIndex p = m_Model->parent(m_Index);

      m_Model->append(p, mod);
    }
  }

  QDialog::accept();
}
