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
    connect(m_IntegrateButton, &QAbstractButton::clicked,
            integ -> dataProcessor().data(), &QxrdDataProcessorThreaded::integrateSaveAndDisplay);

    integ -> prop_Oversample()         -> linkTo(m_OversampleFactor);
    integ -> prop_IntegrationStep()    -> linkTo(m_IntegratorStepSize);
    integ -> prop_IntegrationNSteps()  -> linkTo(m_IntegratorNSteps);
    integ -> prop_IntegrationMinimum() -> linkTo(m_IntegratorMinimum);
    integ -> prop_IntegrationMaximum() -> linkTo(m_IntegratorMaximum);
    integ -> prop_IntegrationXUnits()  -> linkTo(m_IntegratorXUnits);

    integ -> prop_SelfNormalization()  -> linkTo(m_EnableSelfNormalization);
    integ -> prop_SelfNormalizationMinimum() -> linkTo(m_SelfNormalizationMinimum);
    integ -> prop_SelfNormalizationMaximum() -> linkTo(m_SelfNormalizationMaximum);

    integ -> prop_EnableGeometricCorrections() -> linkTo(m_EnableGeometry);
    integ -> prop_EnablePolarizationCorrections() -> linkTo(m_EnablePolarization);
    integ -> prop_Polarization() -> linkTo(m_Polarization);
    integ -> prop_EnableAbsorptionCorrections() -> linkTo(m_EnableAbsorption);
    integ -> prop_AttenuationLength() -> linkTo(m_AttenuationLength);

    integ -> prop_EnableUserAbsorption() -> linkTo(m_EnableUserAbsorption);
    integ -> prop_UserAbsorptionFunction() -> linkTo(m_UserAbsorptionFunction);
    connect(m_UserAbsorptionEdit, &QAbstractButton::clicked, this, &QxrdIntegratorDialog::editUserAbsorption);

    integ -> prop_EnableUserGeometry() -> linkTo(m_EnableUserGeometry);
    integ -> prop_UserGeometryFunction() -> linkTo(m_UserGeometryFunction);
    connect(m_UserGeometryEdit, &QAbstractButton::clicked, this, &QxrdIntegratorDialog::editUserGeometry);
    connect(integ -> prop_EnablePolarizationCorrections(), &QcepBoolProperty::valueChanged, this, &QxrdIntegratorDialog::onEnablePolarizationChanged);
    connect(integ -> prop_EnableAbsorptionCorrections(), &QcepBoolProperty::valueChanged, this, &QxrdIntegratorDialog::onEnableAbsorptionChanged);

    onEnablePolarizationChanged(integ -> get_EnablePolarizationCorrections());
    onEnableAbsorptionChanged(integ -> get_EnableAbsorptionCorrections());
  }
}

QxrdIntegratorDialog::~QxrdIntegratorDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdIntegratorDialog::~QxrdIntegratorDialog(%p)\n", this);
  }
}

void QxrdIntegratorDialog::onEnablePolarizationChanged(bool pol)
{
  m_Polarization      -> setEnabled(pol);
}

void QxrdIntegratorDialog::onEnableAbsorptionChanged(bool absn)
{
  m_AttenuationLength -> setEnabled(absn);
}

void QxrdIntegratorDialog::editUserAbsorption()
{
  QxrdIntegratorPtr integ(m_Integrator);

  if (integ) {
    if (m_EditUserAbsorption == NULL) {
      m_EditUserAbsorption = new QxrdUserScriptDialog("Edit User Absorption Script",
                                                      integ -> experiment(),
                                                      integ -> get_UserAbsorptionScript(),
                                                      integ -> defaultUserAbsorptionScript());

      connect(m_EditUserAbsorption.data(), &QDialog::accepted, this, &QxrdIntegratorDialog::onEditedUserAbsorption);
    }

    m_EditUserAbsorption -> show();
  }
}

void QxrdIntegratorDialog::editUserGeometry()
{
  QxrdIntegratorPtr integ(m_Integrator);

  if (integ) {
    if (m_EditUserGeometry == NULL) {
      m_EditUserGeometry = new QxrdUserScriptDialog("Edit User Geometry Script",
                                                    integ -> experiment(),
                                                    integ -> get_UserGeometryScript(),
                                                    integ -> defaultUserGeometryScript());

      connect(m_EditUserGeometry.data(), &QDialog::accepted, this, &QxrdIntegratorDialog::onEditedUserGeometry);
    }

    m_EditUserGeometry -> show();
  }
}

void QxrdIntegratorDialog::onEditedUserAbsorption()
{
  QxrdIntegratorPtr integ(m_Integrator);

  if (m_EditUserAbsorption && integ) {
    QString script = m_EditUserAbsorption->userScript();

    integ -> set_UserAbsorptionScript(script);
  }
}

void QxrdIntegratorDialog::onEditedUserGeometry()
{
  QxrdIntegratorPtr integ(m_Integrator);

  if (m_EditUserGeometry && integ) {
    QString script = m_EditUserGeometry->userScript();

    integ -> set_UserGeometryScript(script);
  }
}

