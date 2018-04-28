#include "qxrdperkinelmerdialog.h"
#include "qxrdperkinelmersettings.h"

QxrdPerkinElmerDialog::QxrdPerkinElmerDialog(QxrdPerkinElmerSettingsWPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  QxrdPerkinElmerSettingsPtr pset(m_Settings);

  if (pset) {
    setWindowTitle(tr("Configure %1 %2: \"%3\"")
                   .arg(pset->get_DetectorTypeName())
                   .arg(pset->get_DetectorNumber())
                   .arg(pset->get_DetectorName()));

    pset->prop_DetectorNumber()   -> copyTo(m_DetectorNumber);
    pset->prop_DetectorName()     -> copyTo(m_DetectorName);
    pset->prop_DetectorTypeName() -> copyTo(m_DetectorType);

    for (int i=0; i<10; i++) {
      m_DetectorIndex->addItem(tr("%1").arg(i));
    }

    for (int i=0; i<QxrdPerkinElmerSettings::DetectorSubTypeCount; i++) {
      QString typ = QxrdPerkinElmerSettings::detectorSubTypeNamePE(i);
      m_DetectorSubType->addItem(typ);
    }

    QStringList bins = QxrdPerkinElmerSettings::binningModeNamesPE();

    foreach (QString bin, bins) {
      m_DetectorBinning->addItem(bin);
    }

    QStringList gains = QxrdPerkinElmerSettings::gainModeNamesPE();

    foreach (QString gain, gains) {
      m_DetectorGain->addItem(gain);
    }

    QStringList modes = QxrdPerkinElmerSettings::timingModeNamesPE();

    foreach (QString mode, modes) {
      m_DetectorTiming->addItem(mode);
    }

    pset->prop_DetectorIndex()    -> copyTo(m_DetectorIndex);
    pset->prop_DetectorSubType()  -> copyTo(m_DetectorSubType);
    pset->prop_DetectorAddress()  -> copyTo(m_DetectorAddress);
    pset->prop_DetectorGain()     -> copyTo(m_DetectorGain);
    pset->prop_DetectorBinning()  -> copyTo(m_DetectorBinning);
    pset->prop_DetectorTiming()   -> copyTo(m_DetectorTiming);
    pset->prop_ExposureFactor()   -> copyTo(m_ExposureFactor);
    pset->prop_HardwareSync()     -> copyTo(m_HardwareSync);
  }
}

void QxrdPerkinElmerDialog::accept()
{
  QxrdPerkinElmerSettingsPtr pset(m_Settings);

  if (pset) {
    pset->prop_DetectorName()     -> copyFrom(m_DetectorName);

    pset->prop_DetectorIndex()    -> copyFrom(m_DetectorIndex);
    pset->prop_DetectorSubType()  -> copyFrom(m_DetectorSubType);
    pset->prop_DetectorAddress()  -> copyFrom(m_DetectorAddress);
    pset->prop_DetectorGain()     -> copyFrom(m_DetectorGain);
    pset->prop_DetectorBinning()  -> copyFrom(m_DetectorBinning);
    pset->prop_DetectorTiming()   -> copyFrom(m_DetectorTiming);
    pset->prop_ExposureFactor()   -> copyFrom(m_ExposureFactor);
    pset->prop_HardwareSync()     -> copyFrom(m_HardwareSync);
  }

  QxrdDetectorDialog::accept();
}
