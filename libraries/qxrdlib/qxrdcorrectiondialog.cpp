#include "qxrdcorrectiondialog.h"
#include "ui_qxrdcorrectiondialog.h"
#include "qxrddebug.h"
#include "qxrdacqcommon.h"
#include "qxrdprocessor.h"
#include <QMessageBox>
#include <QTabWidget>
#include "qxrdcorrectiondialogpage.h"
#include "qxrddetectorsettings.h"

QxrdCorrectionDialog::QxrdCorrectionDialog(QWidget *parent,
                                           QxrdAcqCommonWPtr acqp,
                                           QxrdProcessorWPtr procw) :
    inherited(parent),
    m_Acquisition(acqp),
    m_Processor(procw)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdCorrectionDialog::QxrdCorrectionDialog(%p)\n", this);
  }

  setupUi(this);

  CONNECT_CHECK(
        connect(m_CorrectionOptionsButton, &QAbstractButton::clicked,
                this, &QxrdCorrectionDialog::doEditPreferences));

  CONNECT_CHECK(connect(m_ApplyToAll, &QCheckBox::toggled, this, &QxrdCorrectionDialog::applyToAll));

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    proc -> prop_PerformDarkSubtraction() -> copyTo(m_PerformDark);
    proc -> prop_SaveRawImages() -> copyTo(m_SaveRaw);
    proc -> prop_SaveDarkImages() -> copyTo(m_SaveDark);
    proc -> prop_PerformBadPixels() -> copyTo(m_PerformBadPixels);
    proc -> prop_PerformGainCorrection() -> copyTo(m_PerformGainCorrection);
    proc -> prop_SaveSubtracted() -> copyTo(m_SaveSubtracted);
    proc -> prop_SaveAsText() -> copyTo(m_SaveAsText);
    proc -> prop_PerformIntegration() -> copyTo(m_PerformIntegration);
    proc -> prop_DisplayIntegratedData() -> copyTo(m_DisplayIntegratedData);
    proc -> prop_SaveIntegratedData() -> copyTo(m_SaveIntegratedData);
    proc -> prop_SaveIntegratedInSeparateFiles() -> copyTo(m_SaveIntegratedInSeparateFiles);
    proc -> prop_AccumulateIntegrated2D() ->copyTo(m_AccumulateIntegrated2D);
  }

  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    acq -> prop_ApplyToAll() -> copyTo(m_ApplyToAll);

    int nDet = acq -> detectorCount();

    for (int i = 0; i<nDet; i++) {
      appendDetectorPage(i, acq->detector(i));
    }
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
  inherited::changeEvent(e);

  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdCorrectionDialog::appendDetectorPage(int i, QxrdDetectorSettingsWPtr det)
{
  QxrdCorrectionDialogPage *page =
      new QxrdCorrectionDialogPage(i, det);

  m_TabWidget -> addTab(page, tr("Det-%1").arg(i));

  m_Pages.append(page);
}

void QxrdCorrectionDialog::accept()
{
  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    proc -> prop_PerformDarkSubtraction() -> copyFrom(m_PerformDark);
    proc -> prop_SaveRawImages() -> copyFrom(m_SaveRaw);
    proc -> prop_SaveDarkImages() -> copyFrom(m_SaveDark);
    proc -> prop_PerformBadPixels() -> copyFrom(m_PerformBadPixels);
    proc -> prop_PerformGainCorrection() -> copyFrom(m_PerformGainCorrection);
    proc -> prop_SaveSubtracted() -> copyFrom(m_SaveSubtracted);
    proc -> prop_SaveAsText() -> copyFrom(m_SaveAsText);
    proc -> prop_PerformIntegration() -> copyFrom(m_PerformIntegration);
    proc -> prop_DisplayIntegratedData() -> copyFrom(m_DisplayIntegratedData);
    proc -> prop_SaveIntegratedData() -> copyFrom(m_SaveIntegratedData);
    proc -> prop_SaveIntegratedInSeparateFiles() -> copyFrom(m_SaveIntegratedInSeparateFiles);
    proc -> prop_AccumulateIntegrated2D() -> copyFrom(m_AccumulateIntegrated2D);
  }

  bool toAll = m_ApplyToAll -> isChecked();

  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    acq -> prop_ApplyToAll() -> copyFrom(m_ApplyToAll);

    int nDet = acq -> detectorCount();

    for (int i = 0; i<nDet; i++) {
      if (toAll) {
        copyDetectorPage(i);
      } else {
        acceptDetectorPage(i);
      }
    }
  }

  inherited::accept();
}

void QxrdCorrectionDialog::applyToAll(bool toAll)
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    int nDet = acq -> detectorCount();

    for (int i = 0; i<nDet; i++) {
      QxrdCorrectionDialogPage *page = m_Pages.value(i);

      if (page) {
        page -> enablePage(!toAll);
      }
    }
  }
}

void QxrdCorrectionDialog::acceptDetectorPage(int i)
{
  QxrdCorrectionDialogPage *page = m_Pages.value(i);

  if (page) {
    page -> accept();
  }
}

void QxrdCorrectionDialog::copyDetectorPage(int i)
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    QxrdDetectorSettingsPtr det = acq->detector(i);

    if (det) {
      QxrdProcessorPtr p2 = det->processor();

      if (p2) {
        p2 -> prop_PerformDarkSubtraction() -> copyFrom(m_PerformDark);
        p2 -> prop_SaveRawImages() -> copyFrom(m_SaveRaw);
        p2 -> prop_SaveDarkImages() -> copyFrom(m_SaveDark);
        p2 -> prop_PerformBadPixels() -> copyFrom(m_PerformBadPixels);
        p2 -> prop_PerformGainCorrection() -> copyFrom(m_PerformGainCorrection);
        p2 -> prop_SaveSubtracted() -> copyFrom(m_SaveSubtracted);
        p2 -> prop_SaveAsText() -> copyFrom(m_SaveAsText);
        p2 -> prop_PerformIntegration() -> copyFrom(m_PerformIntegration);
        p2 -> prop_DisplayIntegratedData() -> copyFrom(m_DisplayIntegratedData);
        p2 -> prop_SaveIntegratedData() -> copyFrom(m_SaveIntegratedData);
        p2 -> prop_SaveIntegratedInSeparateFiles() -> copyFrom(m_SaveIntegratedInSeparateFiles);
        p2 -> prop_AccumulateIntegrated2D() -> copyFrom(m_AccumulateIntegrated2D);
      }
    }
  }
}

void QxrdCorrectionDialog::doEditPreferences()
{
  QMessageBox::information(this, "Edit Preferences", "Edit Preferences");
}
