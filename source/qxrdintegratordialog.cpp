#include "qxrdintegratordialog.h"
#include "qxrdintegrator.h"

QxrdIntegratorDialog::QxrdIntegratorDialog(QxrdIntegratorPtr integ, QWidget *parent)
  : QDockWidget(parent),
    m_Integrator(integ)
{
  setupUi(this);

  connect(m_IntegrateButton, SIGNAL(clicked()),
          m_Integrator -> dataProcessor(), SLOT(integrateSaveAndDisplay()));

  m_Integrator -> prop_Oversample()         -> linkTo(m_OversampleFactor);
  m_Integrator -> prop_IntegrationStep()    -> linkTo(m_IntegratorStepSize);
  m_Integrator -> prop_IntegrationNSteps()  -> linkTo(m_IntegratorNSteps);
  m_Integrator -> prop_IntegrationMinimum() -> linkTo(m_IntegratorMinimum);
  m_Integrator -> prop_IntegrationMaximum() -> linkTo(m_IntegratorMaximum);
  m_Integrator -> prop_IntegrationXUnits()  -> linkTo(m_IntegratorXUnits);
}
