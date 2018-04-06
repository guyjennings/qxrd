#ifndef QXRDDETECTORSETTINGS_H
#define QXRDDETECTORSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepobject.h"
#include <QDoubleSpinBox>
#include <QComboBox>
#include "qcepimagedata-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "qxrdprocessorexecution-ptr.h"
#include "qxrdprocessorexecutionthread-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdappcommon-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrddetectorcontrolwindow-ptr.h"
#include "qxrddetectorcontrolwindowsettings-ptr.h"
#include <QScriptEngine>
#include <QSemaphore>
#include "qcepimagequeue-ptr.h"
#include "qxrdroi-ptr.h"
#include "qxrddetectordriverthread-ptr.h"
#include "qxrddetectordriver-ptr.h"
#include "qxrddetectorplugin-ptr.h"

//TODO: Attach detector output to processor

class QXRD_EXPORT QxrdDetectorSettings : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdDetectorSettings(QString name, int detType);

  void initialize(QcepObjectWPtr parent);

  virtual ~QxrdDetectorSettings();

  static QxrdDetectorSettingsWPtr findDetectorSettings(QcepObjectWPtr p);

  //TODO: rename to newDetectorSettings
  static QxrdDetectorSettingsPtr newDetector(QcepObjectWPtr parent, int detType);

  static void registerMetaTypes();

  static int detectorTypeCount();
  static QString detectorTypeName(int detectorType);
  static QStringList detectorTypeNames();

  QxrdExperimentWPtr experiment();
  QxrdAcqCommonWPtr acquisition();

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdDetectorSettingsWPtr &det);
  static void fromScriptValue(const QScriptValue &obj, QxrdDetectorSettingsWPtr &det);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void enqueueAcquiredFrame(QcepImageDataBasePtr img);

  typedef enum {
    NoDetector,
    Simulated,
    PerkinElmer,
    Pilatus,
    AreaDetector,
    FileWatcher,
    Dexela,
    AlliedVision,
    DetectorTypeCount
  } DetectorType;

  enum {
    FileIndexScaler,
    FilePhaseScaler,
    ExtraScalers
  };

signals:

public slots:
  bool isEnabled();

  bool checkDetectorEnabled();

  void restartDetector();
  void startDetector();
  void stopDetector();

  QxrdDetectorDriver* detectorDriver();

  void changeExposureTime(double expos);
  void beginAcquisition(double exposure);
  void endAcquisition();
  void shutdownAcquisition();

  void openWindow();
  void openControlWindow();
  void closeWindow();

  QxrdProcessorPtr processor();
  void startOrStop(bool enabled);

  QxrdProcessorExecutionThreadPtr processorExecutionThread();

  virtual void beginFrame();
  virtual QcepImageDataBasePtr acquireFrameIfAvailable();
  virtual QcepImageDataBasePtr acquireFrame();
  int availableImageCount();

  int roiCount();
  QxrdROIPtr roi(int i);

  QcepDoubleVector scalerCounts();
  double scalerCounts(int chan);

  QxrdDetectorControlWindowSettingsWPtr detectorControlWindowSettings();
  QxrdDetectorControlWindowWPtr         detectorControlWindow();

  QxrdDetectorDriverPtr createDetector(QString name,
                                       QxrdDetectorSettingsWPtr det,
                                       QxrdExperimentWPtr expt,
                                       QxrdAcqCommonWPtr acq);

  virtual void configureDetector();

protected:
  QxrdAppCommonWPtr                    m_Application;
  QxrdExperimentWPtr                   m_Experiment;
  QxrdAcqCommonWPtr                    m_Acquisition;

  //TODO: should be in detector thread?
  QxrdProcessorPtr                     m_Processor;
  QxrdProcessorExecutionWPtr           m_ProcessorExecution;
  QxrdProcessorExecutionThreadPtr      m_ProcessorExecutionThread;

  QxrdDetectorControlWindowSettingsPtr m_DetectorControlWindowSettings;
  QxrdDetectorControlWindowPtr         m_DetectorControlWindow;

  QxrdDetectorPluginWPtr               m_DetectorPlugin;
  QxrdDetectorDriverThreadPtr          m_DetectorDriverThread;

private:
  QSemaphore                  m_NAcquiredImages;
  QcepImageBaseQueuePtr       m_AcquiredImages;

public:
  Q_PROPERTY(int detectorNumber READ get_DetectorNumber WRITE set_DetectorNumber STORED false)
  QCEP_INTEGER_PROPERTY(DetectorNumber)

  Q_PROPERTY(int detectorIndex READ get_DetectorIndex WRITE set_DetectorIndex)
  QCEP_INTEGER_PROPERTY(DetectorIndex)

  Q_PROPERTY(int detectorType READ get_DetectorType WRITE set_DetectorType)
  QCEP_INTEGER_PROPERTY(DetectorType)

  Q_PROPERTY(QString detectorTypeName READ get_DetectorTypeName WRITE set_DetectorTypeName STORED false)
  QCEP_STRING_PROPERTY(DetectorTypeName)

  Q_PROPERTY(bool enabled READ get_Enabled WRITE set_Enabled)
  QCEP_BOOLEAN_PROPERTY(Enabled)

  Q_PROPERTY(QString detectorName READ get_DetectorName WRITE set_DetectorName)
  QCEP_STRING_PROPERTY(DetectorName)

  Q_PROPERTY(int nCols READ get_NCols WRITE set_NCols STORED false)
  QCEP_INTEGER_PROPERTY(NCols)

  Q_PROPERTY(int nRows READ get_NRows WRITE set_NRows STORED false)
  QCEP_INTEGER_PROPERTY(NRows)

  Q_PROPERTY(int hBinning READ get_HBinning WRITE set_HBinning STORED false)
  QCEP_INTEGER_PROPERTY(HBinning)

  Q_PROPERTY(int vBinning READ get_VBinning WRITE set_VBinning STORED false)
  QCEP_INTEGER_PROPERTY(VBinning)

  Q_PROPERTY(double readoutDelay READ get_ReadoutDelay WRITE set_ReadoutDelay STORED false)
  QCEP_DOUBLE_PROPERTY(ReadoutDelay)

  Q_PROPERTY(int exposureFactor READ get_ExposureFactor WRITE set_ExposureFactor)
  QCEP_INTEGER_PROPERTY(ExposureFactor)

  Q_PROPERTY(bool canHardwareSync READ get_CanHardwareSync WRITE set_CanHardwareSync STORED false)
  QCEP_BOOLEAN_PROPERTY(CanHardwareSync)

  Q_PROPERTY(bool hardwareSync READ get_HardwareSync WRITE set_HardwareSync)
  QCEP_BOOLEAN_PROPERTY(HardwareSync)

  Q_PROPERTY(QString extension READ get_Extension WRITE set_Extension)
  QCEP_STRING_PROPERTY(Extension)
};

#endif // QXRDDETECTORSETTINGS_H
