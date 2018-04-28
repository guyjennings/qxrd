#include "qxrdalliedvisiondialog.h"
#include "qxrdalliedvisionsettings.h"

QxrdAlliedVisionDialog::QxrdAlliedVisionDialog(QxrdAlliedVisionSettingsWPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  QxrdAlliedVisionSettingsPtr avset(m_Settings);

  if (avset) {
    setWindowTitle(tr("Configure %1 %2: \"%3\"")
                   .arg(avset->get_DetectorTypeName())
                   .arg(avset->get_DetectorNumber())
                   .arg(avset->get_DetectorName()));

    avset->prop_DetectorNumber()   -> copyTo(m_DetectorNumber);
    avset->prop_DetectorName()     -> copyTo(m_DetectorName);
    avset->prop_DetectorTypeName() -> copyTo(m_DetectorType);
    avset->prop_ExposureFactor()   -> copyTo(m_ExposureFactor);
  }
}

void QxrdAlliedVisionDialog::accept()
{
  QxrdAlliedVisionSettingsPtr avset(m_Settings);

  if (avset) {
    avset->prop_DetectorName()     -> copyFrom(m_DetectorName);
    avset->prop_ExposureFactor()   -> copyFrom(m_ExposureFactor);
  }

  QxrdDetectorDialog::accept();
}
