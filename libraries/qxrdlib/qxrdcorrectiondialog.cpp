#include "qxrdcorrectiondialog.h"
#include "ui_qxrdcorrectiondialog.h"
#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include "qxrdprocessor.h"
#include <QMessageBox>

QxrdCorrectionDialog::QxrdCorrectionDialog(QWidget *parent,
                                           QxrdAcquisitionWPtr acqp,
                                           QxrdProcessorWPtr procp) :
    QDialog(parent),
    m_Acquisition(acqp),
    m_Processor(procp)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdCorrectionDialog::QxrdCorrectionDialog(%p)\n", this);
  }

  setupUi(this);

  connect(m_CorrectionOptionsButton, &QAbstractButton::clicked,
          this, &QxrdCorrectionDialog::doEditPreferences);

  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq -> prop_RawSaveTime() -> linkTo(m_SaveRawTime);
    acq -> prop_DarkSaveTime() -> linkTo(m_SaveDarkTime);
  }

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    proc -> prop_PerformDarkSubtraction() -> linkTo(m_PerformDark);
    proc -> prop_PerformDarkSubtractionTime() -> linkTo(m_PerformDarkTime);
    proc -> prop_SaveRawImages() -> linkTo(m_SaveRaw);
    proc -> prop_SaveDarkImages() -> linkTo(m_SaveDark);
    proc -> prop_PerformBadPixels() -> linkTo(m_PerformBadPixels);
    proc -> prop_PerformBadPixelsTime() -> linkTo(m_PerformBadPixelsTime);
    proc -> prop_PerformGainCorrection() -> linkTo(m_PerformGainCorrection);
    proc -> prop_PerformGainCorrectionTime() -> linkTo(m_PerformGainCorrectionTime);
    proc -> prop_SaveSubtracted() -> linkTo(m_SaveSubtracted);
    proc -> prop_SaveSubtractedTime() -> linkTo(m_SaveSubtractedTime);
    proc -> prop_SaveAsText() -> linkTo(m_SaveAsText);
    proc -> prop_SaveAsTextTime() -> linkTo(m_SaveAsTextTime);
    proc -> prop_PerformIntegration() -> linkTo(m_PerformIntegration);
    proc -> prop_PerformIntegrationTime() -> linkTo(m_PerformIntegrationTime);
    proc -> prop_DisplayIntegratedData() -> linkTo(m_DisplayIntegratedData);
    proc -> prop_DisplayIntegratedDataTime() -> linkTo(m_DisplayIntegratedDataTime);
    proc -> prop_SaveIntegratedData() -> linkTo(m_SaveIntegratedData);
    proc -> prop_SaveIntegratedDataTime() -> linkTo(m_SaveIntegratedDataTime);
    proc -> prop_SaveIntegratedInSeparateFiles() -> linkTo(m_SaveIntegratedInSeparateFiles);
    proc -> prop_AccumulateIntegrated2D() ->linkTo(m_AccumulateIntegrated2D);
    proc -> prop_AccumulateIntegratedName() -> linkTo(m_AccumulateIntegratedName);
    proc -> prop_EstimatedProcessingTime() -> linkTo(m_EstimatedProcessingTime);
  }
}

QxrdCorrectionDialog::~QxrdCorrectionDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdCorrectionDialog::~QxrdCorrectionDialog(%p)\n", this);
  }
}

void QxrdCorrectionDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);

  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdCorrectionDialog::doEditPreferences()
{
  QMessageBox::information(this, "Edit Preferences", "Edit Preferences");
}
