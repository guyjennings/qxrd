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

      norm -> execute();
    }
  }

  QDialog::accept();
}
