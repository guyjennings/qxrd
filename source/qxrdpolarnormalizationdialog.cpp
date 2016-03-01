#include "qxrdpolarnormalizationdialog.h"
#include "ui_qxrdpolarnormalizationdialog.h"
#include "qxrddataprocessor.h"
#include "qxrdpolarnormalization.h"

QxrdPolarNormalizationDialog::QxrdPolarNormalizationDialog(QxrdDataProcessorWPtr procw, QWidget *parent) :
  QDialog(parent),
  m_Processor(procw)
{
  setupUi(this);

  if (m_Processor) {
    QxrdPolarNormalizationPtr norm = m_Processor->polarNormalization();

    if (norm) {
      m_Source     ->setText(norm->get_Source());
      m_Destination->setText(norm->get_Destination());
      m_Integrated ->setText(norm->get_Integrated());

      m_SelfNormalize->setChecked(norm->get_SelfNormalize());

      m_SelfNormalizeMin->setValue(norm->get_SelfNormalizeMin());
      m_SelfNormalizeMax->setValue(norm->get_SelfNormalizeMax());
    }
  }
}

QxrdPolarNormalizationDialog::~QxrdPolarNormalizationDialog()
{
}

void QxrdPolarNormalizationDialog::accept()
{
  if (m_Processor) {
    QxrdPolarNormalizationPtr norm = m_Processor->polarNormalization();

    if (norm) {
      norm -> set_Source(m_Source->text());
      norm -> set_Destination(m_Destination->text());
      norm -> set_Integrated(m_Integrated->text());

      norm -> set_SelfNormalize(m_SelfNormalize->isChecked());
      norm -> set_SelfNormalizeMin(m_SelfNormalizeMin->value());
      norm -> set_SelfNormalizeMax(m_SelfNormalizeMax->value());

      norm -> execute();
    }
  }

  QDialog::accept();
}
