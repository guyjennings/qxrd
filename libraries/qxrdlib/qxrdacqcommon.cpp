#include "qcepimagedata.h"
#include "qxrdacqcommon.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrddarkacquisitionparameterpack.h"
#include "qxrddetectorsettings.h"
#include "qxrdprocessor.h"
#include "qxrdacquisitionscalermodel.h"
#include "qcepallocator.h"
#include "qxrdappcommon.h"
#include "qxrdacquisitioneventlog.h"

QxrdAcqCommon::QxrdAcqCommon(QString name) :
  inherited(name),
  m_Cancelling(this, "cancelling", 0, "Cancel Acquisition?"),
  m_Triggered(this, "triggered", 0, "Trigger Acquisition"),
  m_ExposureTime(this,"exposureTime",0.1, "Exposure Time (in sec)"),
  m_SkippedExposuresAtStart(this,"skippedExposuresAtStart",0, "Exposures to Skip at Start"),
  m_PhasesInGroup(this,"phasesInGroup",1, "Number of Image Phases"),
  m_CurrentPhase(this, "currentPhase", 0, "Current Acquisition Phase"),
  m_ApplyToAll(this, "applyToAll", false, "Apply processor options to all detectors"),
  m_SummedExposures(this,"summedExposures",1, "Summed Exposures per Image"),
  m_DarkSummedExposures(this,"darkSummedExposures",1, "Summed Exposures in Dark Image"),
  m_CurrentSummation(this, "currentSumation", 0, "Current Acquisition Summation"),
  m_SkippedExposures(this,"skippedExposures",0, "Skipped Exposures between Images"),
  m_PreTriggerFiles(this,"preTriggerFiles",0, "Number of pre-Trigger Images"),
  m_PostTriggerFiles(this,"postTriggerFiles",1, "Number of post-Trigger Images"),
  m_CurrentFile(this, "currentFile", 0, "File Index of Current File"),
  m_FilePattern(this,"filePattern","", "File Name Pattern"),
  m_QxrdVersion(this,"qxrdVersion",g_Application->applicationVersion(), "QXRD Version Number"),
  m_QtVersion(this,"qtVersion",qVersion(), "QT Version Number"),
  //  m_DetectorCount(this, "detectorCount", 0, "Number of Detectors"),
  m_LastAcquired(this, "lastAcquired", 0, "Internal Acquisition Flag"),
  m_FileIndex(this,"fileIndex",0, "File Index"),
  m_FileIndexWidth(this, "fileIndexWidth", 5, "Digits in File Index Field"),
  m_FilePhaseWidth(this, "filePhaseWidth", 3, "Digits in Phase Number Field"),
  m_FileOverflowWidth(this, "fileOverflowWidth", 5, "Digits in Overflow Index Field"),
  m_FileNumberWidth(this, "fileNumberWidth", 5, "Digits in File Number Field"),
  m_DetectorNumberWidth(this, "detectorNumberWidth", 2, "Digits in detector number field"),
  m_FileNameFormat1(this, "fileNameFormat1", 1, "File name format 1st Item"),
  m_FileNameFormat2(this, "fileNameFormat2", 1, "File name format 2nd Item"),
  m_FileNameFormat3(this, "fileNameFormat3", 1, "File name format 3rd Item"),
  m_FileNameFormat4(this, "fileNameFormat4", 1, "File name format 4th Item"),
  m_FileBase(this,"fileBase","", "File Base"),
  m_OverflowLevel(this, "overflowLevel", 65500, "Overflow level (per exposure)"),
  m_Raw16SaveTime(this,"raw16SaveTime", 0.1, "Time to save 16 bit images"),
  m_Raw32SaveTime(this,"raw32SaveTime", 0.2, "Time to save 32 bit images"),
  m_RawSaveTime(this,"rawSaveTime", 0.2, "Time to save raw images"),
  m_DarkSaveTime(this,"darkSaveTime", 0.2, "Time to save dark images"),
  m_UserComment1(this,"userComment1","", "User Comment 1"),
  m_UserComment2(this,"userComment2","", "User Comment 2"),
  m_UserComment3(this,"userComment3","", "User Comment 3"),
  m_UserComment4(this,"userComment4","", "User Comment 4"),
  m_Normalization(this, "normalization", QcepDoubleList(), "Normalization Values"),
  m_DroppedFrames(this,"droppedFrames",0, "Number of Dropped Frames"),
  m_LiveViewAtIdle(this, "liveViewAtIdle", false, "Live View during Idle"),
  m_AcquisitionCancelsLiveView(this, "acquisitionCancelsLiveView", true, "Acquisition operations cancel live view"),
  m_RetryDropped(this, "retryDropped", false, "Automatically retry dropped frames during acquisition"),
  m_ScalerValues(this, "scalerValues", QcepDoubleVector(), "Scaler Values")
{
}

QxrdAcqCommon::~QxrdAcqCommon()
{
}

void QxrdAcqCommon::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  QxrdAcqCommonPtr acq(
        qSharedPointerDynamicCast<QxrdAcqCommon>(sharedFromThis()));

  m_ScalerModel =
      QxrdAcquisitionScalerModelPtr(
        new QxrdAcquisitionScalerModel(acq));

  m_AcquisitionEventLog =
      QxrdAcquisitionEventLogPtr(
        new QxrdAcquisitionEventLog("eventLog"));

  if (m_AcquisitionEventLog) {
    m_AcquisitionEventLog -> initialize(sharedFromThis());
  }
}

QxrdAcqCommonWPtr QxrdAcqCommon::findAcquisition(QcepObjectWPtr p)
{
  QxrdAcqCommonWPtr res =
      qSharedPointerDynamicCast<QxrdAcqCommon>(p);

  if (res == NULL) {
    QcepObjectPtr objp(p);

    if (objp) {
      res = findAcquisition(objp->parentPtr());
    }
  }

  return res;
}

QxrdAcquisitionScalerModelPtr QxrdAcqCommon::acquisitionScalerModel() const
{
  return m_ScalerModel;
}

QxrdAcquisitionEventLogWPtr QxrdAcqCommon::acquisitionEventLog() const
{
  return m_AcquisitionEventLog;
}

QxrdAcquisitionParameterPackPtr QxrdAcqCommon::acquisitionParameterPack()
{
  return QxrdAcquisitionParameterPackPtr(
        new QxrdAcquisitionParameterPack (get_FilePattern(),
                                          get_ExposureTime(),
                                          get_SummedExposures(),
                                          get_PreTriggerFiles(),
                                          get_PostTriggerFiles(),
                                          get_PhasesInGroup(),
                                          get_SkippedExposuresAtStart(),
                                          get_SkippedExposures()));
}

QxrdDarkAcquisitionParameterPackPtr QxrdAcqCommon::darkAcquisitionParameterPack()
{
  return QxrdDarkAcquisitionParameterPackPtr(
        new QxrdDarkAcquisitionParameterPack(get_FilePattern(),
                                          get_ExposureTime(),
                                          get_DarkSummedExposures(),
                                          get_SkippedExposuresAtStart()));

}

int QxrdAcqCommon::activeDetectorCount() const
{
  int nActive = 0;

  for (int i=0; i<detectorCount(); i++) {
    QxrdDetectorSettingsPtr det(detector(i));

    if (det && det->isEnabled()) {
      nActive++;
    }
  }

  return nActive;
}

QString QxrdAcqCommon::fmtString(int f)
{
  QString res = "";

  switch (f) {
  case IndexFormatItem:
    res = "-%2";
    break;

  case DetectorFormatItem:
    res = "-%3";
    break;

  case PhaseFormatItem:
    res = "-%4";
    break;

  case NumberFormatItem:
    res = "-%5";
    break;
  }

  return res;
}

QString QxrdAcqCommon::getFileBaseAndName(QcepImageDataBaseWPtr imgp)
{
  QString res = "";

  int idxWidth = get_FileIndexWidth();
  int detWidth = get_DetectorNumberWidth();
  int phsWidth = get_FilePhaseWidth();
  int numWidth = get_FileNumberWidth();

  int fmt1     = get_FileNameFormat1();
  int fmt2     = get_FileNameFormat2();
  int fmt3     = get_FileNameFormat3();
  int fmt4     = get_FileNameFormat4();

  QcepImageDataBasePtr img(imgp);

  if (img) {
    int idxNum = img->get_ImageSequenceNumber();
    int detNum = img->get_DetectorNumber();
    int phsNum = img->get_PhaseNumber();
    int imgNum = img->get_ImageNumber();
    int nPhase = img->get_NPhases();
    int nImages = img->get_NImages();

    QString basString = img->get_FileBase();

    QString idxString = tr("_%1").arg(idxNum,
                                      idxWidth,
                                      10,
                                      QChar('0'));

    QString detString = tr("_%1").arg(detNum,
                                      detWidth,
                                      10,
                                      QChar('0'));

    QString phsString = tr("_%1").arg(phsNum,
                                      phsWidth,
                                      10,
                                      QChar('0'));

    QString numString = tr("_%1").arg(imgNum,
                                      numWidth,
                                      10,
                                      QChar('0'));

    QString typString = "";

    QString extString = img->get_FileExtension();

    if (img->isDark()) {
      phsString = "";
      numString = "";
      typString = ".dark";
    } else if (img->isRaw()) {
      typString = ".raw";
    }

    if (activeDetectorCount() <= 1) {
      detString = "";
    }

    if (nPhase <= 1) {
      phsString = "";
    }

    if (nImages <= 1) {
      numString = "";
    }

    QString fmt =
        "%1%" + tr("%1").arg(fmt1+1)
        +"%"  + tr("%1").arg(fmt2+1)
        +"%"  + tr("%1").arg(fmt3+1)
        +"%"  + tr("%1").arg(fmt4+1)
        +"%6.%7";

    res = tr(qPrintable(fmt))
        .arg(basString)
        .arg(idxString)
        .arg(detString)
        .arg(phsString)
        .arg(numString)
        .arg(typString)
        .arg(extString);
  }

  return res;
}

QString QxrdAcqCommon::currentFileBase(int detNum, QString extension)
{
  QString fileBase, fileName;

  QxrdDetectorSettingsPtr det = detector(detNum);
  QString extent;

  if (extension.length()) {
    extent = extension;
  } else if (det) {
    extent = det->get_Extension();
  } else {
    extent = "tif";
  }

  fileBase = getFileBaseAndName(QcepImageDataBaseWPtr());

  return fileBase;
}

double QxrdAcqCommon::scalerValue(int i)
{
  return get_ScalerValues().value(i);
}

void QxrdAcqCommon::dynamicProperties()
{
  QByteArray name;

  foreach(name, dynamicPropertyNames()) {
    printMessage(tr("acquisition.%1").arg(name.data()));
  }
}

void QxrdAcqCommon::copyDynamicProperties(QObject *dest)
{
  if (dest) {
    QByteArray name;

    foreach(name, dynamicPropertyNames()) {
      dest -> setProperty(name.data(), property(name.data()));
    }
  }
}

int QxrdAcqCommon::cancelling()
{
  int res = get_Cancelling();

  if (res) {
    printMessage(tr("Cancelling acquisition"));
    statusMessage(tr("Cancelling acquisition"));
  }

  return res;
}

void QxrdAcqCommon::unlock()
{
}

void QxrdAcqCommon::indicateDroppedFrame(int n)
{
  QString msg = tr("Frame %1 dropped [%2/%3 MB Used]")
      .arg(n)
      .arg(QcepAllocator::allocatedMemoryMB())
      .arg(QcepAllocator::availableMemoryMB());

  statusMessage(msg);
  printMessage(msg);

  prop_DroppedFrames() -> incValue(1);
}

void QxrdAcqCommon::clearEventLog()
{
  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    log->clearEventLog();
  }
}

void QxrdAcqCommon::pauseEventLog()
{
  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    log->pauseEventLog();
  }
}

void QxrdAcqCommon::resumeEventLog()
{
  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    log->resumeEventLog();
  }
}

void QxrdAcqCommon::appendEvent(int eventCode,
                                int eventArg1,
                                int eventArg2,
                                QDateTime eventTime)
{
  QxrdAcquisitionEventLogPtr log(m_AcquisitionEventLog);

  if (log) {
    log->appendEvent(eventCode,
                     eventArg1,
                     eventArg2,
                     eventTime);
  }
}
