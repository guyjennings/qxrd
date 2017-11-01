#include "qxrddetectorsettingspilatus.h"
#include "qcepproperty.h"
#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include <QDir>
#include <QFile>
#include "qcepimagedata.h"
#include "qcepallocator.h"
#include "qxrddetectorproxy.h"
#include "qxrddetectorpilatusremote.h"
#include "qxrdexperiment.h"
#include "qxrddataprocessor.h"

QxrdDetectorSettingsPilatus::QxrdDetectorSettingsPilatus(QxrdApplicationWPtr   app,
                                                         QxrdExperimentWPtr    expt,
                                                         QxrdAcquisitionWPtr   acq,
                                                         int                   detNum) :
  QxrdDetectorSettings(app, expt, acq, PilatusDetector, detNum),
  m_PilatusHost            (this, "pilatusHost",          "s11id-pilatus", "Host Address of Computer running Camserver"),
  m_PilatusPort            (this, "pilatusPort",          41234,         "Camserver Port Number"),
  m_PilatusUser            (this, "pilatusUser",          "det",         "Camserver User Name"),
  m_PilatusSSH             (this, "pilatusSSH",           "ssh",         "ssh command path"),
  m_PilatusSCP             (this, "pilatusSCP",           "scp",         "scp command path"),
  m_PilatusDataDirectory   (this, "pilatusDataDirectory", "/home/det/shareddata/test/",    "Data directory on Camserver computer"),
  m_ReadFilesLocally       (this, "readFilesLocally",     true, "Attempt to read acquired files into QXRD for further processing"),
  m_DeleteFilesAfterReading(this, "deleteFilesAfterReading", false, "Delete files from Camserver computer after reading"),
  m_ExposureMode           (this, "exposureMode",         0, "Pilatus Exposure Mode = (0:No Trigger, 1:ExtTrigger, 2:ExtEnable"),
  m_EnableFrequency        (this, "enableFrequency",      1000, "Frequency of ext enable signal"),
  m_PilatusExtension       (this, "pilatusExtension",     "cbf",         "File format to be used by camserver")
{
#ifndef QT_NO_DEBUG
  printf("Constructing Pilatus detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorPilatus::QxrdDetectorPilatus(%p)\n", this);
  }
}

QxrdDetectorSettingsPilatus::~QxrdDetectorSettingsPilatus()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Pilatus detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorPilatus::~QxrdDetectorPilatus(%p)\n", this);
  }
}

void QxrdDetectorSettingsPilatus::pushDefaultsToProxy(QxrdDetectorProxyPtr proxy)
{
  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusHost",          "Camserver Host",    "s11id-pilatus");
    proxy->pushProperty(QxrdDetectorProxy::FixedIntegerProperty,   "pilatusPort",  "Camserver Port",    41234);
    proxy->pushProperty(QxrdDetectorProxy::StringProperty,  "pilatusUser",         "Camserver User",    "det");
    proxy->pushProperty(QxrdDetectorProxy::FilenameProperty,  "pilatusSSH",        "ssh command",       "ssh");
    proxy->pushProperty(QxrdDetectorProxy::FilenameProperty,  "pilatusSCP",        "scp command",       "scp");
    proxy->pushProperty(QxrdDetectorProxy::DirectoryProperty, "pilatusDataDirectory", "Camserver Data Directory", "/home/det/shareddata/test/");
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "readFilesLocally",     "Attempt to read acquired files into QXRD for further processing", true);
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "deleteFilesAfterReading", "Delete files from camserver computer after use", false);
    proxy->pushProperty(QxrdDetectorProxy::ExtensionProperty, "pilatusExtension",   "File format for camserver", "cbf");
    proxy->pushProperty(QxrdDetectorProxy::PilatusModeProperty, "exposureMode",     "Pilatus Exposure Mode", 0);
    proxy->pushProperty(QxrdDetectorProxy::DoubleProperty, "enableFrequency",       "Ext Enable Frequency", 1000);
  }
}

void QxrdDetectorSettingsPilatus::pushPropertiesToProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pushPropertiesToProxy(proxy);

  if (proxy) {
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusHost",          "Camserver Host",    get_PilatusHost());
    proxy->pushProperty(QxrdDetectorProxy::FixedIntegerProperty,   "pilatusPort",  "Camserver Port",    get_PilatusPort());
    proxy->pushProperty(QxrdDetectorProxy::StringProperty, "pilatusUser",          "Camserver User",    get_PilatusUser());
    proxy->pushProperty(QxrdDetectorProxy::FilenameProperty,  "pilatusSSH",        "ssh command",       get_PilatusSSH());
    proxy->pushProperty(QxrdDetectorProxy::FilenameProperty,  "pilatusSCP",        "scp command",       get_PilatusSCP());
    proxy->pushProperty(QxrdDetectorProxy::DirectoryProperty, "pilatusDataDirectory", "Camserver Data Directory", get_PilatusDataDirectory());
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "readFilesLocally",     "Attempt to read acquired files into QXRD for further processing", get_ReadFilesLocally());
    proxy->pushProperty(QxrdDetectorProxy::BooleanProperty, "deleteFilesAfterReading", "Delete files from camserver computer after use", get_DeleteFilesAfterReading());
    proxy->pushProperty(QxrdDetectorProxy::ExtensionProperty, "pilatusExtension",   "File format for camserver", get_PilatusExtension());
    proxy->pushProperty(QxrdDetectorProxy::PilatusModeProperty, "exposureMode",     "Pilatus Exposure Mode", get_ExposureMode());
    proxy->pushProperty(QxrdDetectorProxy::DoubleProperty, "enableFrequency",       "Ext Enable Frequency", get_EnableFrequency());
  }
}

void QxrdDetectorSettingsPilatus::pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy)
{
  QxrdDetectorSettings::pullPropertiesfromProxy(proxy);

  if (proxy) {
    set_PilatusHost            (proxy->property("pilatusHost").toString());
    set_PilatusUser            (proxy->property("pilatusUser").toString());
    set_PilatusSSH             (proxy->property("pilatusSSH").toString());
    set_PilatusSCP             (proxy->property("pilatusSCP").toString());
    set_PilatusDataDirectory   (proxy->property("pilatusDataDirectory").toString());
    set_ReadFilesLocally       (proxy->property("readFilesLocally").toBool());
    set_DeleteFilesAfterReading(proxy->property("deleteFilesAfterReading").toBool());
    set_PilatusExtension       (proxy->property("pilatusExtension").toString());
    set_ExposureMode           (proxy->property("exposureMode").toInt());
    set_EnableFrequency        (proxy->property("enableFrequency").toDouble());
  }
}
