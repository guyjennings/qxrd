#include "qxrdpolartransformdialog.h"
#include "ui_qxrdpolartransformdialog.h"
#include "qxrddataprocessor.h"
#include "qxrdpolartransform.h"

QxrdPolarTransformDialog::QxrdPolarTransformDialog(QxrdDataProcessorWPtr procw, QWidget *parent) :
  QDialog(parent),
  m_Processor(procw)
{
  setupUi(this);

  if (m_Processor) {
    QxrdPolarTransformPtr transform = m_Processor->polarTransform();

    if (transform) {
      m_OutputType->setCurrentIndex(transform->get_OutputType());
      m_Destination->setText(transform->get_Destination());

//      m_PolarType->setCurrentIndex(transform->get_PolarUnits());
      m_PolarStep->setValue(transform->get_PolarStep());
      m_PolarNSteps->setValue(transform->get_PolarNSteps());
      m_PolarStart->setValue(transform->get_PolarStart());
      m_PolarEnd->setValue(transform->get_PolarEnd());

      m_RadialType->setCurrentIndex(transform->get_RadialUnits());
      m_RadialStep->setValue(transform->get_RadialStep());
      m_RadialNSteps->setValue(transform->get_RadialNSteps());
      m_RadialStart->setValue(transform->get_RadialStart());
      m_RadialEnd->setValue(transform->get_RadialEnd());
    }
  }
}

QxrdPolarTransformDialog::~QxrdPolarTransformDialog()
{
}

void QxrdPolarTransformDialog::accept()
{
  if (m_Processor) {
    QxrdPolarTransformPtr transform = m_Processor->polarTransform();

    if (transform) {
      transform->set_OutputType(m_OutputType->currentIndex());
      transform->set_Destination(m_Destination->text());

      transform->set_PolarStep(m_PolarStep->value());
      transform->set_PolarNSteps(m_PolarNSteps->value());
      transform->set_PolarStart(m_PolarStart->value());
      transform->set_PolarEnd(m_PolarEnd->value());

      transform->set_RadialUnits(m_RadialType->currentIndex());
      transform->set_RadialStep(m_RadialStep->value());
      transform->set_RadialNSteps(m_RadialNSteps->value());
      transform->set_RadialStart(m_RadialStart->value());
      transform->set_RadialEnd(m_RadialEnd->value());

      transform->execute();
    }
  }

  QDialog::accept();
}
