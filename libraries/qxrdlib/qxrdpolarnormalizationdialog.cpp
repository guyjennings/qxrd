#include "qxrdpolarnormalizationdialog.h"
#include "ui_qxrdpolarnormalizationdialog.h"
#include "qxrdprocessor.h"
#include "qxrdpolarnormalization.h"

QxrdPolarNormalizationDialog::QxrdPolarNormalizationDialog(QxrdProcessorWPtr procw) :
  QDialog(NULL),
  m_Processor(procw)
{
  setupUi(this);

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdPolarNormalizationPtr norm = proc->polarNormalization();

    if (norm) {
      m_OutputType -> addItem("Integrated Scan");
      m_OutputType -> addItem("Fitted Data");
      m_OutputType -> addItem("Delta Fits");
      m_OutputType -> setCurrentIndex(norm->get_OutputType());

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
  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdPolarNormalizationPtr norm = proc->polarNormalization();

    if (norm) {
      norm -> set_OutputType(m_OutputType->currentIndex());

      norm -> set_SelfNormalize(m_SelfNormalize->isChecked());
      norm -> set_SelfNormalizeMin(m_SelfNormalizeMin->value());
      norm -> set_SelfNormalizeMax(m_SelfNormalizeMax->value());
    }
  }

  QDialog::accept();
}
