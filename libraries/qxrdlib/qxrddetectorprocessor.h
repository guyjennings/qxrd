#ifndef QXRDDETECTORPROCESSOR_H
#define QXRDDETECTORPROCESSOR_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qxrdprocessor.h"

#include "qxrdacquisition-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrddetectorprocessor-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdintegrator-ptr.h"
#include "qxrdroicalculator-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include <QScriptEngine>
#include "qxrddetectorcontrolwindow-ptr.h"
#include "qxrdimageplotsettings-ptr.h"
#include "qxrdfilesaver-ptr.h"

//TODO: eliminate
class QXRD_EXPORT QxrdDetectorProcessor : public QxrdProcessor
{
    Q_OBJECT

public:
  QxrdDetectorProcessor(QxrdExperimentWPtr    doc,
                        QxrdFileSaverWPtr     fsav,
                        QxrdDetectorSettingsWPtr      det);
  virtual ~QxrdDetectorProcessor();

  void setControlWindow(QxrdDetectorControlWindowWPtr ctrl);

signals:

public slots:
  QxrdCenterFinderPtr  centerFinder();
  QxrdIntegratorPtr    integrator();
  QxrdROICalculatorPtr roiCalculator();

  void processAcquiredImage(QcepUInt32ImageDataPtr image,
                            QcepMaskDataPtr overflow,
                            int fileIndex,
                            int phase,
                            int nPhases,
                            bool trig);

  void processDarkImage(QcepUInt32ImageDataPtr image,
                        QcepMaskDataPtr overflow,
                        int fileIndex);

  void processIdleImage(QcepImageDataBasePtr image);

  QcepImageDataBasePtr   data();
  QcepUInt32ImageDataPtr  dark();
  QcepDoubleImageDataPtr badPixels();
  QcepDoubleImageDataPtr gainCorrection();
  QcepMaskDataPtr        mask();
  QcepMaskDataPtr        overflow();

  QcepDataObjectPtr integrate(QcepDoubleImageDataPtr img);
  QcepDataObjectPtr polarTransform(QcepDoubleImageDataPtr img);
  QcepDataObjectPtr polarIntegrate(QcepDoubleImageDataPtr img);

  bool integrateParameters();
  bool polarTransformParameters();
  bool polarIntegrateParameters();

public:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdDetectorProcessorPtr &proc);
  static void fromScriptValue(const QScriptValue &obj, QxrdDetectorProcessorPtr &proc);

  QxrdImagePlotSettingsWPtr imagePlotSettings();

  enum {
    NoDisplayMode,
    ImageDisplayMode,
    IntegratedDisplayMode
  };

  QString              filePathInDarkOutputDirectory(QString fileName) const;
  QString              filePathInRawOutputDirectory(QString fileName) const;
  QString              filePathInSubtractedOutputDirectory(QString fileName) const;
  QString              filePathInIntegratedOutputDirectory(QString fileName) const;

private slots:
  void onMaskPathChanged(QString newPath);
  void onDarkImagePathChanged(QString newPath);
  void onBadPixelsPathChanged(QString newPath);
  void onGainMapPathChanged(QString newPath);

private:
  void setAcquiredImageProperties(QcepImageDataBasePtr image,
                                  int fileIndex,
                                  int phase,
                                  int nPhases,
                                  bool trig);

  QcepImageDataBasePtr doDarkSubtraction    (QcepImageDataBasePtr img);
  QcepImageDataBasePtr doBadPixels          (QcepImageDataBasePtr img);
  QcepImageDataBasePtr doGainCorrection     (QcepImageDataBasePtr img);
  QcepDoubleVector     doCalculateROICounts (QcepImageDataBasePtr img);
  void                 doSaveRawImage       (QcepImageDataBasePtr img, QcepMaskDataPtr ovf);
  void                 doSaveSubtractedImage(QcepImageDataBasePtr img, QcepMaskDataPtr ovf);
  void                 doSaveDarkImage      (QcepImageDataBasePtr img, QcepMaskDataPtr ovf);

  QString              dataDirectory() const;
  QString              darkOutputDirectory() const;
  QString              rawOutputDirectory() const;
  QString              subtractedOutputDirectory() const;
  QString              integratedOutputDirectory() const;
  QString              existingOutputDirectory(QString dir, QString subdir) const;

public:
  // Properties...

  Q_PROPERTY(int detectorDisplayMode READ get_DetectorDisplayMode WRITE set_DetectorDisplayMode)
  QCEP_INTEGER_PROPERTY(DetectorDisplayMode)

  Q_PROPERTY(bool performDarkSubtraction READ get_PerformDarkSubtraction WRITE set_PerformDarkSubtraction)
  QCEP_BOOLEAN_PROPERTY(PerformDarkSubtraction)

  Q_PROPERTY(bool saveRawImages READ get_SaveRawImages WRITE set_SaveRawImages)
  QCEP_BOOLEAN_PROPERTY(SaveRawImages)

  Q_PROPERTY(bool saveDarkImages READ get_SaveDarkImages WRITE set_SaveDarkImages)
  QCEP_BOOLEAN_PROPERTY(SaveDarkImages)

  Q_PROPERTY(bool performBadPixels READ get_PerformBadPixels WRITE set_PerformBadPixels)
  QCEP_BOOLEAN_PROPERTY(PerformBadPixels)

  Q_PROPERTY(bool performGainCorrection READ get_PerformGainCorrection WRITE set_PerformGainCorrection)
  QCEP_BOOLEAN_PROPERTY(PerformGainCorrection)

  Q_PROPERTY(bool saveSubtracted READ get_SaveSubtracted WRITE set_SaveSubtracted)
  QCEP_BOOLEAN_PROPERTY(SaveSubtracted)

  Q_PROPERTY(QString darkImagePath   READ get_DarkImagePath WRITE set_DarkImagePath)
  QCEP_STRING_PROPERTY(DarkImagePath)

  Q_PROPERTY(QString rawDataSubdir READ get_RawDataSubdir WRITE set_RawDataSubdir)
  QCEP_STRING_PROPERTY(RawDataSubdir)

  Q_PROPERTY(QString darkDataSubdir READ get_DarkDataSubdir WRITE set_DarkDataSubdir)
  QCEP_STRING_PROPERTY(DarkDataSubdir)

  Q_PROPERTY(QString badPixelsPath   READ get_BadPixelsPath WRITE set_BadPixelsPath)
  QCEP_STRING_PROPERTY(BadPixelsPath)

  Q_PROPERTY(QString gainMapPath     READ get_GainMapPath WRITE set_GainMapPath)
  QCEP_STRING_PROPERTY(GainMapPath)

  Q_PROPERTY(QString subtractedSubdir READ get_SubtractedSubdir WRITE set_SubtractedSubdir)
  QCEP_STRING_PROPERTY(SubtractedSubdir)

  Q_PROPERTY(QString maskPath     READ get_MaskPath WRITE set_MaskPath)
  QCEP_STRING_PROPERTY(MaskPath)


  Q_PROPERTY(bool performIntegration READ get_PerformIntegration WRITE set_PerformIntegration)
  QCEP_BOOLEAN_PROPERTY(PerformIntegration)

  Q_PROPERTY(bool displayIntegratedData READ get_DisplayIntegratedData WRITE set_DisplayIntegratedData)
  QCEP_BOOLEAN_PROPERTY(DisplayIntegratedData)

  Q_PROPERTY(bool saveIntegratedData READ get_SaveIntegratedData WRITE set_SaveIntegratedData)
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedData)

  Q_PROPERTY(QString integratedDataFile READ get_IntegratedDataFile WRITE set_IntegratedDataFile)
  QCEP_STRING_PROPERTY(IntegratedDataFile)

  Q_PROPERTY(bool saveIntegratedDataSeparate READ get_SaveIntegratedDataSeparate WRITE set_SaveIntegratedDataSeparate)
  QCEP_BOOLEAN_PROPERTY(SaveIntegratedDataSeparate)

  Q_PROPERTY(QString integratedDataSubdir READ get_IntegratedDataSubdir WRITE set_IntegratedDataSubdir)
  QCEP_STRING_PROPERTY(IntegratedDataSubdir)

  Q_PROPERTY(bool accumulateIntegrated2D READ get_AccumulateIntegrated2D WRITE set_AccumulateIntegrated2D)
  QCEP_BOOLEAN_PROPERTY(AccumulateIntegrated2D)

  Q_PROPERTY(QString accumulateIntegratedName READ get_AccumulateIntegratedName WRITE set_AccumulateIntegratedName)
  QCEP_STRING_PROPERTY(AccumulateIntegratedName)

  Q_PROPERTY(bool calculateROICounts READ get_CalculateROICounts WRITE set_CalculateROICounts)
  QCEP_BOOLEAN_PROPERTY(CalculateROICounts)

  Q_PROPERTY(bool displayROIBorders READ get_DisplayROIBorders WRITE set_DisplayROIBorders)
  QCEP_BOOLEAN_PROPERTY(DisplayROIBorders)

  Q_PROPERTY(QcepDoubleVector roiCounts READ get_RoiCounts WRITE set_RoiCounts STORED false)
  QCEP_DOUBLE_VECTOR_PROPERTY(RoiCounts)

private:
  QMutex                m_Mutex;

  QxrdExperimentWPtr    m_Experiment;
  QxrdFileSaverWPtr     m_FileSaver;
  QxrdDetectorSettingsWPtr      m_Detector;

  QxrdCenterFinderPtr   m_CenterFinder;
  QxrdIntegratorPtr     m_Integrator;
  QxrdROICalculatorPtr  m_ROICalculator;

  QxrdDetectorControlWindowWPtr m_ControlWindow;

  QxrdImagePlotSettingsPtr m_ImagePlotSettings;

  QcepImageDataBasePtr   m_Data;
  QcepUInt32ImageDataPtr  m_DarkImage;
  QcepDoubleImageDataPtr m_BadPixels;
  QcepDoubleImageDataPtr m_GainMap;
  QcepDoubleImageDataPtr m_LiveData;
  QcepMaskDataPtr        m_Mask;
  QcepMaskDataPtr        m_Overflow;
};

#endif // QXRDACQUISITIONPROCESSOR_H
