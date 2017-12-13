#include "qxrdperkinelmerdialog.h"
#include "qxrdperkinelmersettings.h"

QxrdPerkinElmerDialog::QxrdPerkinElmerDialog(
    QxrdPerkinElmerSettingsPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  if (m_Settings) {
    setWindowTitle(tr("Configure %1 %2: \"%3\"")
                   .arg(m_Settings->get_DetectorTypeName())
                   .arg(m_Settings->get_DetectorNumber())
                   .arg(m_Settings->get_DetectorName()));

    m_Settings->prop_DetectorNumber()   -> copyTo(m_DetectorNumber);
    m_Settings->prop_DetectorName()     -> copyTo(m_DetectorName);
    m_Settings->prop_DetectorTypeName() -> copyTo(m_DetectorType);

    m_Settings->prop_DetectorIndex()    -> copyTo(m_DetectorIndex);
    m_Settings->prop_DetectorSubType()  -> copyTo(m_DetectorSubType);
    m_Settings->prop_DetectorAddress()  -> copyTo(m_DetectorAddress);
    m_Settings->prop_DetectorGain()     -> copyTo(m_DetectorGain);
    m_Settings->prop_DetectorBinning()  -> copyTo(m_DetectorBinning);
    m_Settings->prop_DetectorTiming()   -> copyTo(m_DetectorTiming);
  }
}

void QxrdPerkinElmerDialog::accept()
{
  if (m_Settings) {
    m_Settings->prop_DetectorName()     -> copyFrom(m_DetectorName);

    m_Settings->prop_DetectorIndex()    -> copyFrom(m_DetectorIndex);
    m_Settings->prop_DetectorSubType()  -> copyFrom(m_DetectorSubType);
    m_Settings->prop_DetectorAddress()  -> copyFrom(m_DetectorAddress);
    m_Settings->prop_DetectorGain()     -> copyFrom(m_DetectorGain);
    m_Settings->prop_DetectorBinning()  -> copyFrom(m_DetectorBinning);
    m_Settings->prop_DetectorTiming()   -> copyFrom(m_DetectorTiming);
  }

  QxrdDetectorDialog::accept();
}
