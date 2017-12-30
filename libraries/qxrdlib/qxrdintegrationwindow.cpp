#include "qxrdintegrationwindow.h"
#include "qxrdexperiment.h"
#include "qxrdintegrator.h"
#include "qxrddataprocessor.h"
#include "qxrdintegrationwindowsettings.h"

QxrdIntegrationWindow::QxrdIntegrationWindow(QxrdIntegrationWindowSettingsWPtr set,
                                             QString name,
                                             QxrdApplicationWPtr app,
                                             QxrdExperimentWPtr expt) :
  QxrdMainWindow(name, app, expt),
  m_IntegrationWindowSettings(set)
{
  setupUi(this);

  setupMenus(m_FileMenu, m_EditMenu, m_WindowMenu);

  m_Splitter->setStretchFactor(0, 1);
  m_Splitter->setStretchFactor(1, 5);
  m_Splitter->setStretchFactor(2, 1);

  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    m_Integrator = exp->integrator();
  }

  QxrdIntegratorPtr integ(m_Integrator);

  if (integ) {
    connect(m_IntegrateButton, &QAbstractButton::clicked,
            integ -> dataProcessor().data(), &QxrdDataProcessor::integrateSaveAndDisplay);

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

    connect(m_UserAbsorptionEdit, &QAbstractButton::clicked,
            this, &QxrdIntegrationWindow::editUserAbsorption);

    integ -> prop_EnableUserGeometry() -> linkTo(m_EnableUserGeometry);
    integ -> prop_UserGeometryFunction() -> linkTo(m_UserGeometryFunction);
    connect(m_UserGeometryEdit, &QAbstractButton::clicked,
            this, &QxrdIntegrationWindow::editUserGeometry);

    connect(integ -> prop_EnablePolarizationCorrections(), &QcepBoolProperty::valueChanged,
            this, &QxrdIntegrationWindow::onEnablePolarizationChanged);

    connect(integ -> prop_EnableAbsorptionCorrections(), &QcepBoolProperty::valueChanged,
            this, &QxrdIntegrationWindow::onEnableAbsorptionChanged);

    onEnablePolarizationChanged(integ -> get_EnablePolarizationCorrections());
    onEnableAbsorptionChanged(integ -> get_EnableAbsorptionCorrections());
  }

  m_DatasetBrowserView -> setExperiment(expt);

  if (exp) {
    QcepDatasetModelPtr model(exp->dataset());

    m_DatasetBrowserView -> setDatasetModel(model);

    QxrdDataProcessorPtr proc(exp->dataProcessor());

    QxrdIntegrationWindowSettingsPtr settings(m_IntegrationWindowSettings);

    if (settings) {
      m_FileBrowserWidget     -> initialize(settings->fileBrowserSettings(), exp, proc);
      m_ImagePlotWidget       -> initialize(settings->imagePlotWidgetSettings());
      m_IntegratedPlotWigdget -> initialize(settings->integratedPlotWidgetSettings());
    }
  }
}

QxrdIntegrationWindow::~QxrdIntegrationWindow()
{
}

void QxrdIntegrationWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdIntegrationWindow::onEnablePolarizationChanged(bool pol)
{
  m_Polarization      -> setEnabled(pol);
}

void QxrdIntegrationWindow::onEnableAbsorptionChanged(bool absn)
{
  m_AttenuationLength -> setEnabled(absn);
}

void QxrdIntegrationWindow::editUserAbsorption()
{
  QxrdIntegratorPtr integ(m_Integrator);

  if (integ) {
    if (m_EditUserAbsorption == NULL) {
      m_EditUserAbsorption = new QxrdUserScriptDialog("Edit User Absorption Script",
                                                      integ -> experiment(),
                                                      integ -> get_UserAbsorptionScript(),
                                                      integ -> defaultUserAbsorptionScript());

      connect(m_EditUserAbsorption.data(), &QDialog::accepted,
              this, &QxrdIntegrationWindow::onEditedUserAbsorption);
    }

    m_EditUserAbsorption -> show();
  }
}

void QxrdIntegrationWindow::editUserGeometry()
{
  QxrdIntegratorPtr integ(m_Integrator);

  if (integ) {
    if (m_EditUserGeometry == NULL) {
      m_EditUserGeometry = new QxrdUserScriptDialog("Edit User Geometry Script",
                                                    integ -> experiment(),
                                                    integ -> get_UserGeometryScript(),
                                                    integ -> defaultUserGeometryScript());

      connect(m_EditUserGeometry.data(), &QDialog::accepted,
              this, &QxrdIntegrationWindow::onEditedUserGeometry);
    }

    m_EditUserGeometry -> show();
  }
}

void QxrdIntegrationWindow::onEditedUserAbsorption()
{
  QxrdIntegratorPtr integ(m_Integrator);

  if (m_EditUserAbsorption && integ) {
    QString script = m_EditUserAbsorption->userScript();

    integ -> set_UserAbsorptionScript(script);
  }
}

void QxrdIntegrationWindow::onEditedUserGeometry()
{
  QxrdIntegratorPtr integ(m_Integrator);

  if (m_EditUserGeometry && integ) {
    QString script = m_EditUserGeometry->userScript();

    integ -> set_UserGeometryScript(script);
  }
}

