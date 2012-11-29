#include "qxrdintegratordialog.h"
#include "qxrdintegrator.h"
#include "qxrddataprocessor.h"
#include "qxrddebug.h"

QxrdIntegratorDialog::QxrdIntegratorDialog(QxrdIntegratorWPtr integw, QWidget *parent)
  : QDockWidget(parent),
    m_Integrator(integw)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdIntegratorDialog::QxrdIntegratorDialog(%p)\n", this);
  }

  setupUi(this);

  QxrdIntegratorPtr integ(m_Integrator);

  if (integ) {
    connect(m_IntegrateButton, SIGNAL(clicked()),
            integ -> dataProcessor().data(), SLOT(integrateSaveAndDisplay()));

    integ -> prop_Oversample()         -> linkTo(m_OversampleFactor);
    integ -> prop_IntegrationStep()    -> linkTo(m_IntegratorStepSize);
    integ -> prop_IntegrationNSteps()  -> linkTo(m_IntegratorNSteps);
    integ -> prop_IntegrationMinimum() -> linkTo(m_IntegratorMinimum);
    integ -> prop_IntegrationMaximum() -> linkTo(m_IntegratorMaximum);
    integ -> prop_IntegrationXUnits()  -> linkTo(m_IntegratorXUnits);
  }
}

QxrdIntegratorDialog::~QxrdIntegratorDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdIntegratorDialog::~QxrdIntegratorDialog(%p)\n", this);
  }
}
