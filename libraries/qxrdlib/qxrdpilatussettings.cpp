#include "qxrdpilatussettings.h"
#include "qxrdpilatussettings-ptr.h"
#include "qcepproperty.h"
#include "qxrddebug.h"
#include "qxrdacquisition.h"
#include <QDir>
#include <QFile>
#include "qcepimagedata.h"
#include "qcepallocator.h"
#include "qxrdexperiment.h"
#include "qxrdpilatusdialog.h"
#include "qxrdpilatusdialog-ptr.h"

QxrdPilatusSettings::QxrdPilatusSettings(QString name) :
  QxrdDetectorSettings(name, Pilatus),
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

QxrdPilatusSettings::~QxrdPilatusSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Pilatus detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorPilatus::~QxrdDetectorPilatus(%p)\n", this);
  }
}

void QxrdPilatusSettings::configureDetector()
{
  GUI_THREAD_CHECK;

  QxrdPilatusSettingsPtr myself =
      qSharedPointerDynamicCast<QxrdPilatusSettings>(sharedFromThis());

  QxrdPilatusDialogPtr dlog =
      QxrdPilatusDialogPtr(
        new QxrdPilatusDialog(myself));

  if (dlog) {
    dlog->exec();
  }
}
