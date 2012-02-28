#ifndef QXRDACQUISITIONPARAMETERS_H
#define QXRDACQUISITIONPARAMETERS_H

#include "qcepmacros.h"

#include <QObject>
#include <QMutex>

#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitiontrigger.h"
#include "qxrdacquisitiontrigger-ptr.h"
#include "qxrdacquisitiontriggerthread.h"
#include "qxrdacquisitiontriggerthread-ptr.h"
#include "qcepproperty.h"
#include "qxrdsettingssaver-ptr.h"

class QxrdAcquisitionParameters : public QObject
{
  Q_OBJECT

public:
  typedef enum {
    SimulatedDetector,
    PerkinElmerDetector,
    PilatusDetector,
    EpicsAreaDetector,
    FileWatcherDetector
  } DetectorKind;

  QxrdAcquisitionParameters(DetectorKind detectorKind, QxrdSettingsSaverWPtr saver);
  virtual ~QxrdAcquisitionParameters();

public slots:
  void dynamicProperties();
  void updateSaveTimes();

//  void setDynamicProperty(QString name, QVariant value);
//  QVariant dynamicProperty(QString name);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void copyDynamicProperties(QObject *dest);

  QString detectorKindName(DetectorKind detectorKind);

protected:
  enum { MegaBytes = 0x100000 };

  QxrdSettingsSaverWPtr m_Saver;

public:
  Q_PROPERTY(QString qxrdVersion READ get_QxrdVersion STORED false)
  QCEP_STRING_PROPERTY(QxrdVersion)

  Q_PROPERTY(QString qtVersion READ get_QtVersion STORED false)
  QCEP_STRING_PROPERTY(QtVersion)

  Q_PROPERTY(int    detectorType     READ get_DetectorType WRITE set_DetectorType STORED false)
  QCEP_INTEGER_PROPERTY(DetectorType)

  Q_PROPERTY(QString detectorTypeName READ get_DetectorTypeName WRITE set_DetectorTypeName STORED false)
  QCEP_STRING_PROPERTY(DetectorTypeName)

  Q_PROPERTY(double exposureTime     READ get_ExposureTime WRITE set_ExposureTime)
  QCEP_DOUBLE_PROPERTY(ExposureTime)

  Q_PROPERTY(int    skippedExposuresAtStart  READ get_SkippedExposuresAtStart WRITE set_SkippedExposuresAtStart)
  QCEP_INTEGER_PROPERTY(SkippedExposuresAtStart)

  Q_PROPERTY(int    lastAcquired  READ get_LastAcquired WRITE set_LastAcquired STORED false)
  QCEP_INTEGER_PROPERTY(LastAcquired)

  Q_PROPERTY(int    phasesInGroup  READ get_PhasesInGroup WRITE set_PhasesInGroup)
  QCEP_INTEGER_PROPERTY(PhasesInGroup)

  Q_PROPERTY(int    summedExposures  READ get_SummedExposures WRITE set_SummedExposures)
  QCEP_INTEGER_PROPERTY(SummedExposures)

  Q_PROPERTY(int    skippedExposures  READ get_SkippedExposures WRITE set_SkippedExposures)
  QCEP_INTEGER_PROPERTY(SkippedExposures)

  Q_PROPERTY(int    preTriggerFiles  READ get_PreTriggerFiles WRITE set_PreTriggerFiles)
  QCEP_INTEGER_PROPERTY(PreTriggerFiles)

  Q_PROPERTY(int    postTriggerFiles  READ get_PostTriggerFiles WRITE set_PostTriggerFiles)
  QCEP_INTEGER_PROPERTY(PostTriggerFiles)

  Q_PROPERTY(int    fileIndex        READ get_FileIndex WRITE set_FileIndex)
  QCEP_INTEGER_PROPERTY(FileIndex)

  Q_PROPERTY(QString filePattern     READ get_FilePattern WRITE set_FilePattern)
  QCEP_STRING_PROPERTY(FilePattern)

  Q_PROPERTY(int    fileIndexWidth        READ get_FileIndexWidth WRITE set_FileIndexWidth)
  QCEP_INTEGER_PROPERTY(FileIndexWidth)

  Q_PROPERTY(int    filePhaseWidth        READ get_FilePhaseWidth WRITE set_FilePhaseWidth)
  QCEP_INTEGER_PROPERTY(FilePhaseWidth)

  Q_PROPERTY(int    fileOverflowWidth        READ get_FileOverflowWidth WRITE set_FileOverflowWidth)
  QCEP_INTEGER_PROPERTY(FileOverflowWidth)

  Q_PROPERTY(int     darkSummedExposures READ get_DarkSummedExposures WRITE set_DarkSummedExposures)
  QCEP_INTEGER_PROPERTY(DarkSummedExposures)

  Q_PROPERTY(int     cameraGain      READ get_CameraGain WRITE set_CameraGain)
  QCEP_INTEGER_PROPERTY(CameraGain)

  Q_PROPERTY(int     binningMode      READ get_BinningMode WRITE set_BinningMode)
  QCEP_INTEGER_PROPERTY(BinningMode)

  Q_PROPERTY(QString fileBase        READ get_FileBase WRITE set_FileBase)
  QCEP_STRING_PROPERTY(FileBase)

  Q_PROPERTY(int     nRows      READ get_NRows WRITE set_NRows)
  QCEP_INTEGER_PROPERTY(NRows)

  Q_PROPERTY(int     nCols      READ get_NCols WRITE set_NCols)
  QCEP_INTEGER_PROPERTY(NCols)

  Q_PROPERTY(int     overflowLevel      READ get_OverflowLevel WRITE set_OverflowLevel)
  QCEP_INTEGER_PROPERTY(OverflowLevel)

  Q_PROPERTY(int     acquireDark      READ get_AcquireDark WRITE set_AcquireDark STORED false)
  QCEP_INTEGER_PROPERTY(AcquireDark)

  Q_PROPERTY(int     cancelling      READ get_Cancelling WRITE set_Cancelling STORED false)
  QCEP_INTEGER_PROPERTY(Cancelling)

  Q_PROPERTY(int     triggered      READ get_Triggered WRITE set_Triggered STORED false)
  QCEP_INTEGER_PROPERTY(Triggered)

  Q_PROPERTY(double     raw16SaveTime    READ get_Raw16SaveTime WRITE set_Raw16SaveTime)
  QCEP_DOUBLE_PROPERTY(Raw16SaveTime)

  Q_PROPERTY(double     raw32SaveTime    READ get_Raw32SaveTime WRITE set_Raw32SaveTime)
  QCEP_DOUBLE_PROPERTY(Raw32SaveTime)

  Q_PROPERTY(double     rawSaveTime    READ get_RawSaveTime WRITE set_RawSaveTime)
  QCEP_DOUBLE_PROPERTY(RawSaveTime)

  Q_PROPERTY(double     darkSaveTime    READ get_DarkSaveTime WRITE set_DarkSaveTime)
  QCEP_DOUBLE_PROPERTY(DarkSaveTime)

  Q_PROPERTY(QString userComment1 READ get_UserComment1 WRITE set_UserComment1)
  QCEP_STRING_PROPERTY(UserComment1)

  Q_PROPERTY(QString userComment2 READ get_UserComment2 WRITE set_UserComment2)
  QCEP_STRING_PROPERTY(UserComment2)

  Q_PROPERTY(QString userComment3 READ get_UserComment3 WRITE set_UserComment3)
  QCEP_STRING_PROPERTY(UserComment3)

  Q_PROPERTY(QString userComment4 READ get_UserComment4 WRITE set_UserComment4)
  QCEP_STRING_PROPERTY(UserComment4)

  Q_PROPERTY(int     droppedFrames    READ get_DroppedFrames WRITE set_DroppedFrames STORED false)
  QCEP_INTEGER_PROPERTY(DroppedFrames)

protected:
  mutable QMutex                m_Mutex;

  QxrdSynchronizedAcquisitionPtr  m_SynchronizedAcquisition;
  QxrdAcquisitionTriggerThreadPtr m_AcquisitionTriggerThread;
  QxrdAcquisitionTriggerPtr       m_AcquisitionTrigger;
};

#endif // QXRDACQUISITIONPARAMETERS_H
