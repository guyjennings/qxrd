#include "qxrdfilewatcherdialog.h"
#include "qxrdfilewatchersettings.h"

QxrdFileWatcherDialog::QxrdFileWatcherDialog(QxrdFileWatcherSettingsWPtr set, QWidget *parent) :
  QxrdDetectorDialog(parent),
  m_Settings(set)
{
  setupUi(this);

  QxrdFileWatcherSettingsPtr wset(m_Settings);

  if (wset) {
    setWindowTitle(tr("Configure %1 %2: \"%3\"")
                   .arg(wset->get_DetectorTypeName())
                   .arg(wset->get_DetectorNumber())
                   .arg(wset->get_DetectorName()));

    wset->prop_DetectorNumber()   -> copyTo(m_DetectorNumber);
    wset->prop_DetectorName()     -> copyTo(m_DetectorName);
    wset->prop_DetectorTypeName() -> copyTo(m_DetectorType);
    wset->prop_ExposureFactor()   -> copyTo(m_ExposureFactor);
  }
}

void QxrdFileWatcherDialog::accept()
{
  QxrdFileWatcherSettingsPtr wset(m_Settings);

  if (wset) {
    wset->prop_DetectorName()     -> copyFrom(m_DetectorName);
    wset->prop_ExposureFactor()   -> copyFrom(m_ExposureFactor);
  }

  QxrdDetectorDialog::accept();
}
