#include "qxrdalliedvisionplugin.h"
#include "qxrdalliedvisiondriver.h"
#include "qxrdalliedvisionsettings.h"
#include "qxrdalliedvisionplugin-ptr.h"
#include "VimbaCPP/Include/VimbaCPP.h"

#define VMB_ERROR_CHECK(functionCall) do { VmbErrorType error; \
                                       if((error=(functionCall)) != VmbErrorSuccess ) { \
                                           QxrdAlliedVisionPlugin::errorCheck(__FILE__,__LINE__,error); \
                                           goto Error; \
                                       } } while(0)

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

  for(AVT::VmbAPI::CameraPtrVector::const_iterator cam = cameras.begin(); cam != cameras.end(); ++cam) {
    std::string strID, strName, strModelName, strSerialNumber, strInterfaceID;

    AVT::VmbAPI::CameraPtr cp = *cam;

    VMB_ERROR_CHECK(cp->GetID(strID));
    VMB_ERROR_CHECK(cp->GetName(strName));
    VMB_ERROR_CHECK(cp->GetModel(strModelName));
    VMB_ERROR_CHECK(cp->GetSerialNumber(strSerialNumber));
    VMB_ERROR_CHECK(cp->GetInterfaceID(strInterfaceID));

    printMessage(tr("ID:%1;NM:%2;MD:%3;SN:%4;IF:%5")
                 .arg(QString::fromStdString(strID))
                 .arg(QString::fromStdString(strName))
                 .arg(QString::fromStdString(strModelName))
                 .arg(QString::fromStdString(strSerialNumber))
                 .arg(QString::fromStdString(strInterfaceID)));
  }

Error:
  return;
}

void QxrdAlliedVisionPlugin::errorCheck(const char *file, int line, int err)
{
  if (err != VmbErrorSuccess) {
    printMessage(tr("%1:%2 Vimba Error %3").arg(file).arg(line).arg(err));
  }
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
