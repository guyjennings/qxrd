#include "qxrdintegratorparmsdialog.h"
#include "ui_qxrdintegratorparmsdialog.h"
#include "qxrdprocessor.h"
#include "qxrdintegrator.h"

QxrdIntegratorParmsDialog::QxrdIntegratorParmsDialog(QxrdProcessorWPtr procw) :
  QDialog(NULL),
  m_Processor(procw)
{
  setupUi(this);

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdIntegratorPtr integ = proc->integrator();

    if (integ) {
      m_RadialType->setCurrentIndex(integ->get_IntegrationXUnits());
      m_RadialStep->setValue(integ->get_IntegrationStep());
      m_RadialNSteps->setValue(integ->get_IntegrationNSteps());
      m_RadialStart->setValue(integ->get_IntegrationMinimum());
      m_RadialEnd->setValue(integ->get_IntegrationMaximum());

      m_Oversample->setValue(integ->get_Oversample());

      m_EnableGeometry->setChecked(integ->get_EnableGeometricCorrections());
      m_EnablePolarization->setChecked(integ->get_EnablePolarizationCorrections());
      m_Polarization->setValue(integ->get_Polarization());
    }
  }
}

QxrdIntegratorParmsDialog::~QxrdIntegratorParmsDialog()
{
}

void QxrdIntegratorParmsDialog::accept()
{
  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QxrdIntegratorPtr integ = proc->integrator();

    if (integ) {
      integ->set_IntegrationXUnits(m_RadialType->currentIndex());
      integ->set_IntegrationStep(m_RadialStep->value());
      integ->set_IntegrationNSteps(m_RadialNSteps->value());
      integ->set_IntegrationMinimum(m_RadialStart->value());
      integ->set_IntegrationMaximum(m_RadialEnd->value());

      integ->set_Oversample(m_Oversample->value());

      integ->set_EnableGeometricCorrections(m_EnableGeometry->isChecked());
      integ->set_EnablePolarizationCorrections(m_EnablePolarization->isChecked());
      integ->set_Polarization(m_Polarization->value());
    }
  }

  QDialog::accept();
}
