#include "qxrdcorrectiondialogpage.h"
#include "qxrddetectorsettings.h"
#include "qxrdprocessor.h"

QxrdCorrectionDialogPage::QxrdCorrectionDialogPage(int i,
                                                   QxrdDetectorSettingsWPtr detw) :
  QWidget(),
  m_Detector(detw)
{
  setupUi(this);

  QxrdDetectorSettingsPtr det(m_Detector);

  if (det) {
    m_DetectorName -> setText(det->get_DetectorName());

    det -> prop_Enabled() -> copyTo(m_DetectorEnabled);

    m_Processor = det->processor();

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
  }
}

QxrdCorrectionDialogPage::~QxrdCorrectionDialogPage()
{
}

void QxrdCorrectionDialogPage::accept()
{
  QxrdDetectorSettingsPtr det(m_Detector);

  if (det) {
    det -> prop_Enabled() -> copyFrom(m_DetectorEnabled);

    m_Processor = det->processor();

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
      proc -> prop_AccumulateIntegrated2D() ->copyFrom(m_AccumulateIntegrated2D);
    }
  }
}

void QxrdCorrectionDialogPage::enablePage(bool e)
{
//  m_DetectorEnabled -> setEnabled(e);
  m_PerformDark                   -> setEnabled(e);
  m_SaveRaw                       -> setEnabled(e);
  m_SaveDark                      -> setEnabled(e);
  m_PerformBadPixels              -> setEnabled(e);
  m_PerformGainCorrection         -> setEnabled(e);
  m_SaveSubtracted                -> setEnabled(e);
  m_SaveAsText                    -> setEnabled(e);
  m_PerformIntegration            -> setEnabled(e);
  m_DisplayIntegratedData         -> setEnabled(e);
  m_SaveIntegratedData            -> setEnabled(e);
  m_SaveIntegratedInSeparateFiles -> setEnabled(e);
  m_AccumulateIntegrated2D        -> setEnabled(e);
}
