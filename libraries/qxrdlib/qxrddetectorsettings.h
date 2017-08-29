#ifndef QXRDDETECTORSETTINGS_H
#define QXRDDETECTORSETTINGS_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qcepserializableobject.h"
#include <QDoubleSpinBox>
#include <QComboBox>
#include "qcepimagedata-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrddetectorproxy-ptr.h"
#include "qxrddetectorprocessor-ptr.h"
#include "qxrddetectorcontrolwindow-ptr.h"
#include "qxrddetectorcontrolwindowsettings-ptr.h"
#include <QScriptEngine>
#include <QSemaphore>
#include "qcepimagequeue.h"
#include "qxrdroicoordinates-ptr.h"
#include "qxrddetectordriverthread-ptr.h"

class QxrdDetectorSettings : public QcepSerializableObject
{
  Q_OBJECT

public:
  explicit QxrdDetectorSettings(QxrdExperimentWPtr    expt,
                                QxrdAcquisitionWPtr   acq,
                                int                   detType,
                                int                   detNum);

  void initialize();
  virtual ~QxrdDetectorSettings();

  static QxrdDetectorSettingsPtr newDetector(QxrdExperimentWPtr    expt,
                                             QxrdAcquisitionWPtr   acq,
                                             int                   detType,
                                             int                   detNum);

  virtual void printLine(QString line);
  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime());
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

  static int detectorTypeCount();
  static QString detectorTypeName(int detectorType);
  static QStringList detectorTypeNames();

  QxrdExperimentWPtr experiment();
  QxrdAcquisitionWPtr acquisition();

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdDetectorSettingsWPtr &det);
  static void fromScriptValue(const QScriptValue &obj, QxrdDetectorSettingsWPtr &det);

  virtual void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  virtual void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);

  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy, int detType);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void enqueueAcquiredFrame(QcepImageDataBasePtr img);

  typedef enum {
    NoDetector,
    SimulatedDetector,
    PerkinElmerDetector,
    PilatusDetector,
    EpicsAreaDetector,
    FileWatcherDetector
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

  bool startDetector();
  bool stopDetector();

  bool changeExposureTime(double expos);
  bool beginAcquisition(double exposure);
  bool endAcquisition();
  bool shutdownAcquisition();

  void openWindow();
  void openControlWindow();

  QxrdDetectorProcessorPtr processor();
  void startOrStop(bool enabled);

  virtual void beginFrame();
  virtual QcepImageDataBasePtr acquireFrameIfAvailable();
  virtual QcepImageDataBasePtr acquireFrame();

  int roiCount();
  QxrdROICoordinatesPtr roi(int i);

  QcepDoubleVector scalerCounts();
  double scalerCounts(int chan);

  QxrdDetectorControlWindowSettingsWPtr detectorControlWindowSettings();
  QxrdDetectorControlWindowWPtr         detectorControlWindow();

protected:
  QxrdExperimentWPtr                   m_Experiment;
  QxrdAcquisitionWPtr                  m_Acquisition;
  QxrdDetectorProcessorPtr             m_Processor;
  QxrdDetectorControlWindowSettingsPtr m_DetectorControlWindowSettings;
  QxrdDetectorControlWindowPtr         m_DetectorControlWindow;

  QxrdDetectorDriverThreadPtr          m_DetectorDriver;

private:
  QMutex                      m_Mutex;
  QSemaphore                  m_NAcquiredImages;
  QcepImageBaseQueue          m_AcquiredImages;

public:
  Q_PROPERTY(int detectorNumber READ get_DetectorNumber WRITE set_DetectorNumber STORED false)
  QCEP_INTEGER_PROPERTY(DetectorNumber)

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

  Q_PROPERTY(QString extension READ get_Extension WRITE set_Extension)
  QCEP_STRING_PROPERTY(Extension)
};

#endif // QXRDDETECTORSETTINGS_H
