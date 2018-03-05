#include "qxrdpilatusdialog.h"
#include "qxrdpilatussettings.h"

QxrdPilatusDialog::QxrdPilatusDialog(
    QxrdPilatusSettingsPtr set, QWidget *parent) :
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

    m_Settings->prop_PilatusHost()             -> copyTo(m_PilatusHost);
    m_Settings->prop_PilatusUser()             -> copyTo(m_PilatusUser);
    m_Settings->prop_PilatusSSH()              -> copyTo(m_PilatusSSH);
    m_Settings->prop_PilatusSCP ()             -> copyTo(m_PilatusSCP);
    m_Settings->prop_PilatusDataDirectory()    -> copyTo(m_PilatusDataDirectory);
    m_Settings->prop_ReadFilesLocally ()       -> copyTo(m_ReadFilesLocally);
    m_Settings->prop_DeleteFilesAfterReading() -> copyTo(m_DeleteFilesAfterReading);
    m_Settings->prop_PilatusExtension()        -> copyTo(m_PilatusExtension);
    m_Settings->prop_ExposureMode()            -> copyTo(m_ExposureMode);
    m_Settings->prop_EnableFrequency()         -> copyTo(m_EnableFrequency);
    m_Settings->prop_ExposureFactor()          -> copyTo(m_ExposureFactor);
    m_Settings->prop_HardwareSync()            -> copyTo(m_HardwareSync);
  }
}

void QxrdPilatusDialog::accept()
{
  if (m_Settings) {
    m_Settings->prop_DetectorName()            -> copyFrom(m_DetectorName);

    m_Settings->prop_PilatusHost()             -> copyFrom(m_PilatusHost);
    m_Settings->prop_PilatusUser()             -> copyFrom(m_PilatusUser);
    m_Settings->prop_PilatusSSH()              -> copyFrom(m_PilatusSSH);
    m_Settings->prop_PilatusSCP ()             -> copyFrom(m_PilatusSCP);
    m_Settings->prop_PilatusDataDirectory()    -> copyFrom(m_PilatusDataDirectory);
    m_Settings->prop_ReadFilesLocally ()       -> copyFrom(m_ReadFilesLocally);
    m_Settings->prop_DeleteFilesAfterReading() -> copyFrom(m_DeleteFilesAfterReading);
    m_Settings->prop_PilatusExtension()        -> copyFrom(m_PilatusExtension);
    m_Settings->prop_ExposureMode()            -> copyFrom(m_ExposureMode);
    m_Settings->prop_EnableFrequency()         -> copyFrom(m_EnableFrequency);
    m_Settings->prop_ExposureFactor()          -> copyFrom(m_ExposureFactor);
    m_Settings->prop_HardwareSync()            -> copyFrom(m_HardwareSync);
  }

  QxrdDetectorDialog::accept();
}
