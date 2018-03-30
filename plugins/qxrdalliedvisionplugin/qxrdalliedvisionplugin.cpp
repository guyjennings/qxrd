#include "qxrdalliedvisionplugin.h"
#include "qxrdalliedvisiondriver.h"
#include "qxrdalliedvisionsettings.h"
#include "qxrdalliedvisionplugin-ptr.h"

QxrdAlliedVisionPlugin::QxrdAlliedVisionPlugin()
  : inherited("alliedVisionPlugin"),
    m_Vimba(AVT::VmbAPI::VimbaSystem::GetInstance())
{
}

void QxrdAlliedVisionPlugin::registerMetaTypes()
{
  qRegisterMetaType<QxrdAlliedVisionPlugin*>("QxrdAlliedVisionPlugin*");
  qRegisterMetaType<QxrdAlliedVisionDriver*>("QxrdAlliedVisionDriver*");
}

void QxrdAlliedVisionPlugin::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  printMessage("QxrdAlliedVisionPlugin::initialize");

  AVT::VmbAPI::CameraPtrVector cameras;

  auto err1 = m_Vimba.Startup();
  auto err2 = m_Vimba.GetCameras(cameras);

  int nCameras = cameras.size();

  printMessage(tr("Found %1 Allied Vision Cameras").arg(nCameras));
}

QString QxrdAlliedVisionPlugin::name() const
{
  return "Allied Vision Detector Plugin";
}

QxrdDetectorDriverPtr QxrdAlliedVisionPlugin::createDetector(QString name,
                                                          QxrdDetectorSettingsWPtr det,
                                                          QxrdExperimentWPtr expt,
                                                          QxrdAcqCommonWPtr acq)
{
  QxrdAlliedVisionPluginPtr plugin(
        qSharedPointerDynamicCast<QxrdAlliedVisionPlugin>(sharedFromThis()));

  QxrdAlliedVisionSettingsPtr set(
        qSharedPointerDynamicCast<QxrdAlliedVisionSettings>(det));

  QxrdDetectorDriverPtr res =
      QxrdDetectorDriverPtr(new QxrdAlliedVisionDriver(name, plugin, set, expt, acq));

  return res;
}
