#include "qxrdintegratordialog.h"
#include "qxrdintegrator.h"

QxrdIntegratorDialog::QxrdIntegratorDialog(QxrdIntegratorPtr integ, QWidget *parent)
  : QWidget(parent),
    m_Integrator(integ)
{
  setupUi(this);

  connect(m_IntegrateButton, SIGNAL(clicked()),
          m_Integrator -> dataProcessor(), SLOT(integrateSaveAndDisplay()));

  m_Integrator -> prop_Oversample()      -> linkTo(m_OversampleFactor);
  m_Integrator -> prop_IntegrationStep() -> linkTo(m_IntegratorStepSize);
}
